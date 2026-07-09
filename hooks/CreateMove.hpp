//
// Created by sh1rm4n on 5/17/26.
//

#ifndef CENTURY_CREATEMOVE_HPP
#define CENTURY_CREATEMOVE_HPP

#include <polyhook2/Detour/x64Detour.hpp>

#include "../core/globals.hpp"

#include "../sourcesdk/UserCmd.hpp"
#include "../core/utils.hpp"

#include "../hacks/Aimbot.hpp"
#include "../hacks/AntiAim.hpp"
#include "../hacks/BunnyHop.hpp"

inline std::uint64_t ogCreateMove = 0;

bool dtCreateMove(ClientModeGModNormal* vmt, UserCmd* cmd, float inputSampleTime) {
    static auto ogFunction = PLH::FnCast(ogCreateMove, dtCreateMove);
    const auto result = ogFunction(vmt, cmd, inputSampleTime);

    if (!cmd || cmd->commandNumber == 0)
        return result;

    globals::core::currentCmd = cmd;

    const auto cmdMaxTickDelta = (1.f / interfaces->globalVars->interval_per_tick) * 2.5f;
    const auto cmdLowestPossible = std::max(0.0, interfaces->globalVars->tickcount - cmdMaxTickDelta);
    const auto cmdHighestPossible = interfaces->globalVars->tickcount + cmdMaxTickDelta;

    const auto perfectTick = static_cast<int>(std::floor(cmdHighestPossible) - 1);

    //interfaces->globalVars->tickcount = perfectTick;
    //cmd->tickCount = perfectTick;

    cmd->isTyping = true; // salut mode

    bunnyhop->Run(cmd);

    antiAim->Run(cmd);

    if (globals::hacks::fakelag::enabled) {
        static auto fakeLagCounter = 0;
        if (fakeLagCounter >= globals::hacks::fakelag::ticks) {
            utils::SetSendPacket(true);
            fakeLagCounter = 0;
        }
        else {
            if (globals::hacks::fakelag::method == bSendPacket) {
                utils::SetSendPacket(false);
            }
            else if (globals::hacks::fakelag::method == Choking) {
                utils::ChokePacket();
            }
            else if (globals::hacks::fakelag::method == bSendPacketWithChoking) {
                utils::ChokePacket();
                utils::SetSendPacket(false);
            }
            fakeLagCounter++;
        }

    }

    if (globals::core::enginePrediction)
        interfaces->prediction->RunCommand(globals::core::localPlayer, cmd, interfaces->moveHelper);

    if (globals::hacks::aimbot::enabled)
        aimbot->Run(cmd);         // angles set first

    if (cmd->tickCount <= cmdLowestPossible && cmd->tickCount > cmdHighestPossible) {
        utils::Warn("[century] UserCmd tick count is wrong! ({})", cmd->tickCount);
        utils::Warn("[century] Only possible tick counts: Highest: {}; Lowest: {}", cmdHighestPossible, cmdLowestPossible);
    }

    return false;
};

#endif //CENTURY_CREATEMOVE_HPP
