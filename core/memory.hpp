//
// Created by sh1rm4n on 5/17/26.
//

#ifndef CENTURY_MEMORY_HPP
#define CENTURY_MEMORY_HPP

#include <cstdint>
#include <fstream>
#include <vector>
#include <dlfcn.h>
#include <link.h>

namespace memory {
    struct Region {
        uintptr_t start, end;
    };

    inline const std::uintptr_t ResolveGlobal(const std::uintptr_t &instruction) {
        auto ripOffset = *reinterpret_cast<std::int32_t *>(instruction + 3);
        return instruction + 7 + ripOffset;
    }

    inline void *GetVMT(const std::uintptr_t &pattern) {
        auto globalAddr = ResolveGlobal(pattern);
        return *reinterpret_cast<void **>(globalAddr);
    }

    template<typename T>
    inline T GetVMTFunc(void *vmt, const std::int32_t &index) {
        auto vtable = *reinterpret_cast<void ***>(vmt);
        return reinterpret_cast<T>(vtable[index]);
    }

    inline std::uint64_t GetAbsAddress(const std::uintptr_t &inst, std::uintptr_t instOffset = 3,
                                       std::uintptr_t instSize = 7) {
        auto offset = *reinterpret_cast<std::int32_t *>(inst + instOffset);
        auto rip = inst + instSize;

        return rip + static_cast<std::uint64_t>(offset);
    }

    template<typename Return, typename... Arguments>
    inline Return Call(void *vmt, const std::uint32_t index, Arguments... args) noexcept {
        using Function = Return(*)(void *, decltype(args)...);
        return (*static_cast<Function **>(vmt))[index](vmt, args...);
    }

    inline std::vector<int> PatternToBytes(const char *pattern) noexcept {
        std::vector<int> bytes;

        const char *cur = pattern;

        while (*cur) {
            if (*cur == '?') {
                ++cur;

                if (*cur == '?')
                    ++cur;

                bytes.push_back(-1);
            } else {
                bytes.push_back(strtoul(cur, nullptr, 16));

                while (*cur && *cur != ' ')
                    ++cur;
            }

            while (*cur == ' ')
                ++cur;
        }

        return bytes;
    }

    inline uintptr_t FindPattern(
        const char *module,
        const char *pattern) noexcept {
        auto bytes = PatternToBytes(pattern);

        std::ifstream maps("/proc/self/maps");

        std::string line;

        while (std::getline(maps, line)) {
            if (line.find(module) == std::string::npos)
                continue;

            uintptr_t start, end;

            sscanf(
                line.c_str(),
                "%lx-%lx",
                &start,
                &end);

            uint8_t *mem = (uint8_t *) start;

            size_t size = end - start;

            for (size_t i = 0; i < size - bytes.size(); i++) {
                bool found = true;

                for (size_t j = 0; j < bytes.size(); j++) {
                    if (
                        bytes[j] != -1 &&
                        mem[i + j] != bytes[j]
                    ) {
                        found = false;
                        break;
                    }
                }

                if (found)
                    return start + i;
            }
        }

        return 0;
    }

    inline std::uintptr_t ResolvePLT(const std::uintptr_t &pltAddress) noexcept {
        if (!pltAddress)
            return 0;

        auto *ptr = reinterpret_cast<uint8_t *>(pltAddress);

        if (ptr[0] == 0xFF && ptr[1] == 0x25) {
            auto offset = *reinterpret_cast<std::int32_t *>(ptr + 2);

            std::uintptr_t gotEntryAddress = pltAddress + 6 + offset;

            std::uintptr_t realAddress = *reinterpret_cast<std::uintptr_t *>(gotEntryAddress);

            return realAddress;
        }

        if (ptr[0] == 0xF3 && ptr[1] == 0x0F && ptr[2] == 0x1E && ptr[3] == 0xFA) {
            return ResolvePLT(pltAddress + 4);
        }

        return 0;
    }

    inline std::uint64_t ResolveGOT( const char* module, uintptr_t gotOffset )
    {
        const auto handle = dlopen( module, RTLD_NOW | RTLD_NOLOAD );
        if (!handle)
            return 0;

        link_map* lm;
        dlinfo( handle, RTLD_DI_LINKMAP, &lm );

        std::uint64_t base = lm->l_addr;
        dlclose( handle );

        return *reinterpret_cast<std::uint64_t*>( base + gotOffset );
    }
}

#endif //CENTURY_MEMORY_HPP
