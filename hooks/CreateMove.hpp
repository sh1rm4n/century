//
// Created by sh1rm4n on 5/17/26.
//

#ifndef CENTURY_CREATEMOVE_HPP
#define CENTURY_CREATEMOVE_HPP

#pragma once

#include <polyhook2/Detour/x64Detour.hpp>

#include "../core/globals.hpp"
#include "../core/luaexecution.hpp"

#include "../sourcesdk/CUserCmd.hpp"

#include "../hacks/Aimbot.hpp"
#include "../hacks/AntiAim.hpp"
#include "../hacks/BunnyHop.hpp"
#include "../hacks/FakeLag.hpp"
#include "../hacks/NoSpread.hpp"

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

    const int nCmdMaxTickDelta = ( 1.f / interfaces->globalVars->intervalPerTick ) * 2.5f;
    const int nMinDelta = std::max( 0, interfaces->globalVars->tickCount - nCmdMaxTickDelta );
    const int nMaxDelta = interfaces->globalVars->tickCount + nCmdMaxTickDelta;

    //cmd->tickCount = utils::Random(nMinDelta, nMaxDelta - 1);

    bunnyhop->Run(cmd);

    //antiAim->Run(cmd);

    if (globals::Hacks::FakeLag::enabled && globals::Hacks::FakeLag::mode == Static)
        fakeLag->Run(globals::Hacks::FakeLag::ticks);

    if (globals::Core::enginePrediction)
        interfaces->prediction->RunCommand(globals::Core::localPlayer, cmd, interfaces->moveHelper);

    if (globals::Hacks::Aimbot::enabled)
        aimbot->Run(cmd);         // angles set first

    noSpread->Run(cmd);

    return false;
};

#endif //CENTURY_CREATEMOVE_HPP
