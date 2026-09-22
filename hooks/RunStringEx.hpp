//
// Created by sh1rm4n on 7/28/26.
//

#ifndef CENTURY_RUNSTRINGEX_HPP
#define CENTURY_RUNSTRINGEX_HPP

#pragma once

#include <polyhook2/Detour/x64Detour.hpp>

#include "../core/utils.hpp"

#include "../hacks/ScriptHook.hpp"

#include "../sourcesdk/interfaces/CLuaInterface.hpp"

class CLuaShared;
class CLuaInterface;

inline std::uint64_t ogCreateLuaInterface = 0;

inline std::unique_ptr<PLH::x64Detour> hkRunStringEx = nullptr;
inline std::uint64_t ogRunStringEx = 0;

inline std::uint64_t ogCloseLuaInterface = 0;

int dtRunStringEx(CLuaInterface* luaInterface, const char* fileName, const char* path, const char* stringToRun, bool run, bool printErrors, bool dontPushErrors, bool noReturns) {
    static auto ogFunction = PLH::FnCast(ogRunStringEx, dtRunStringEx);

    if (fileName && std::string(fileName) != "0") {
        utils::Warn("fileName: {}", fileName);
        scriptHook->Dump(std::string(fileName), std::string(stringToRun));
    }

    return ogFunction(luaInterface, fileName, path, stringToRun, run, printErrors, dontPushErrors, noReturns);
}

CLuaInterface* dtCreateLuaInterface(CLuaShared* vmt, int realm, bool renew) {
    static auto ogFunction = PLH::FnCast(ogCreateLuaInterface, dtCreateLuaInterface);

    const auto result = ogFunction(vmt, realm, renew);

    if (!hkRunStringEx || result->GetRealm() == 0) {
        hkRunStringEx = std::make_unique<PLH::x64Detour>(
            memory::GetVMTFunc<std::uint64_t>(result, RUNSTRINGEX_INDEX),
            reinterpret_cast<std::uint64_t>(dtRunStringEx), &ogRunStringEx);

        if (hkRunStringEx->hook())
            utils::Warn("successfully hooked RunStringEx");
    }

    return result;
}

int dtCloseLuaInterface(CLuaShared* vmt, CLuaInterface* luaInterface) {
    static auto ogFunction = PLH::FnCast(ogCloseLuaInterface, dtCloseLuaInterface);

    if (hkRunStringEx || luaInterface->GetRealm() == 0) {
        if (hkRunStringEx->unHook())
            utils::Warn("successfully unhooked RunStringEx");
    }

    utils::Warn("close lua interface for: luaInterface 0x{:X}", reinterpret_cast<std::uint64_t>(luaInterface));

    return ogFunction(vmt, luaInterface);
}

#endif //CENTURY_RUNSTRINGEX_HPP
