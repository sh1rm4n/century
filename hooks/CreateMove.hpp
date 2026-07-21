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

bool dtCreateMove(ClientModeGModNormal* vmt, CUserCmd* cmd, float inputSampleTime) {
    static auto ogFunction = PLH::FnCast(ogCreateMove, dtCreateMove);
    const auto result = ogFunction(vmt, cmd, inputSampleTime);

    if (!cmd || cmd->commandNumber == 0)
        return result;

    globals::Core::currentCmd = cmd;

    const auto cmdMaxTickDelta = (1.f / interfaces->globalVars->intervalPerTick) * 2.5f;
    const auto cmdLowestPossible = std::max(0.0, interfaces->globalVars->tickCount - cmdMaxTickDelta);
    const auto cmdHighestPossible = interfaces->globalVars->tickCount + cmdMaxTickDelta;

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

    if (globals::Core::enginePrediction)
        interfaces->prediction->RunCommand(globals::Core::localPlayer, cmd, interfaces->moveHelper);

    if (globals::Hacks::FakeLag::enabled && globals::Hacks::FakeLag::mode == Static)
        fakeLag->Run(globals::Hacks::FakeLag::ticks);

    if (globals::Hacks::Aimbot::enabled)
        aimbot->Run(cmd);         // angles set first

    if (cmd->tickCount <= cmdLowestPossible || cmd->tickCount > cmdHighestPossible) {
        utils::Warn("[century] UserCmd tick count is wrong! ({})", cmd->tickCount);
        utils::Warn("[century] Only possible tick counts: Highest: {}; Lowest: {}", cmdHighestPossible, cmdLowestPossible);
    }

    utils::Warn("cmdLowestPossible: {}", cmdLowestPossible);

    return false;
};

#endif //CENTURY_CREATEMOVE_HPP
