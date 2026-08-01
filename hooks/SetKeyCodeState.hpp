//
// Created by sh1rm4n on 5/24/26.
//

#ifndef CENTURY_SETKEYCODESTATE_HPP
#define CENTURY_SETKEYCODESTATE_HPP

#pragma once

#include <polyhook2/Detour/x64Detour.hpp>

inline std::uint64_t ogSetKeyCodeState = 0;

void dtSetKeyCodeState(CInputInternal* vmt, KeyCode code, bool down) {
    static auto ogFunction = PLH::FnCast(ogSetKeyCodeState, dtSetKeyCodeState);

    if (code == KEY_HOME && down)
        globals::Menu::active = true;

    ogFunction(vmt, code, down);
}

#endif //CENTURY_SETKEYCODESTATE_HPP
