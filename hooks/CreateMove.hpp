//
// Created by sh1rm4n on 5/17/26.
//

#ifndef CENTURY_CREATEMOVE_HPP
#define CENTURY_CREATEMOVE_HPP

#include <climits>
#include <polyhook2/Detour/x64Detour.hpp>

#include "../core/globals.hpp"

#include "../sourcesdk/CUserCmd.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"

#include "../core/utils.hpp"

#include "../hacks/Aimbot.hpp"
#include "../hacks/AntiAim.hpp"
#include "../hacks/BunnyHop.hpp"
#include "../hacks/FakeLag.hpp"

inline std::uint64_t ogCreateMove = 0;

#if defined(__linux__)
bool dtCreateMove(ClientModeGModNormal* vmt, CUserCmd* cmd, float inputSampleTime) {
#else
bool dtCreateMove(ClientModeGModNormal* vmt, float inputSampleTime, CUserCmd* cmd) {
#endif
    //utils::Warn("createmove");

    static auto ogFunction = PLH::FnCast(ogCreateMove, dtCreateMove);
#if defined(__linux__)
    const auto result = ogFunction(vmt, cmd, inputSampleTime);
#else
    const auto result = ogFunction(vmt, inputSampleTime, cmd);
#endif

    if (!cmd || cmd->commandNumber == 0)
        return result;

    globals::Core::currentCmd = cmd;

    cmd->isTyping = true;

    if (interfaces->inputSystem->IsButtonDown(KEY_F))
        cmd->impulse = 100;

    if (cmd->buttons & IN_USE) {
        const auto shouldSpam = (cmd->commandNumber & 2);
        if (shouldSpam == 0) {
            cmd->buttons &= ~IN_USE;
        }
        else {
            cmd->buttons |= IN_USE;
        }
    }

    bunnyhop->Run(cmd);

    antiAim->Run(cmd);

    if (globals::Hacks::FakeLag::enabled && globals::Hacks::FakeLag::mode == Static)
        fakeLag->Run(globals::Hacks::FakeLag::ticks);

    if (globals::Core::enginePrediction)
        interfaces->prediction->RunCommand(globals::Core::localPlayer, cmd, interfaces->moveHelper);

    if (globals::Hacks::Aimbot::enabled)
        aimbot->Run(cmd);         // angles set first

    return false;
};

#endif //CENTURY_CREATEMOVE_HPP
