//
// Created by sh1rm4n on 5/18/26.
//

#ifndef CENTURY_PUMPWINDOWSMESSAGELOOP_HPP
#define CENTURY_PUMPWINDOWSMESSAGELOOP_HPP

#pragma once

#include <polyhook2/Detour/x64Detour.hpp>

inline std::uint64_t ogPumpWindowsMessageLoop = 0;

void dtPumpWindowsMessageLoop(CSDLMgr* vmt) {
    static auto ogFunction = PLH::FnCast(ogPumpWindowsMessageLoop, dtPumpWindowsMessageLoop);

    if (globals::Menu::active) {
        interfaces->inputSystem->EnableInput(true);

        return;
    }

    ogFunction(vmt);
}

#endif //CENTURY_PUMPWINDOWSMESSAGELOOP_HPP
