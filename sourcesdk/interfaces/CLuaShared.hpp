//
// Created by sh1rm4n on 6/13/26.
//

#ifndef CENTURY_CLUASHARED_HPP
#define CENTURY_CLUASHARED_HPP

#include <cstdint>

class CLuaShared {
public:
    constexpr std::uint64_t GetLuaInterface(const int realm) noexcept {
#if defined(__linux__)
        return memory::Call<std::uint64_t>(this, 7, realm);
#else
        return memory::Call<std::uint64_t>(this, 6, realm);
#endif
    }
};

#endif //CENTURY_CLUASHARED_HPP
