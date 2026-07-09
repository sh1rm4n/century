//
// Created by sh1rm4n on 5/18/26.
//

#ifndef CENTURY_PUMPWINDOWSMESSAGELOOP_HPP
#define CENTURY_PUMPWINDOWSMESSAGELOOP_HPP

#include <polyhook2/Detour/x64Detour.hpp>

inline std::uint64_t ogPumpWindowsMessageLoop = 0;

void dtPumpWindowsMessageLoop() {
    static auto ogFunction = PLH::FnCast(ogPumpWindowsMessageLoop, dtPumpWindowsMessageLoop);

    if (globals::menu::active) {
        interfaces->inputSystem->EnableInput(!globals::menu::active);

        return;
    }

    ogFunction();
}

#endif //CENTURY_PUMPWINDOWSMESSAGELOOP_HPP
