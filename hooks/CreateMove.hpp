//
// Created by sh1rm4n on 5/17/26.
//

#ifndef CENTURY_CREATEMOVE_HPP
#define CENTURY_CREATEMOVE_HPP

#include <polyhook2/Detour/x64Detour.hpp>

#include "../core/globals.hpp"

#include "../sourcesdk/UserCmd.hpp"
#include "../sourcesdk/typedefs.hpp"
#include "../core/utils.hpp"

#include "../hacks/BunnyHop.hpp"
#include "../hacks/Aimbot.hpp"

inline std::uint64_t ogCreateMove = 0;

void AntiAimRun(UserCmd* cmd) noexcept {
    if (!globals::core::localPlayer || !cmd || !globals::core::localPlayer->IsAlive()) {
        utils::SetSendPacket(true);
        return;
    }

    // always send on attack
    if (cmd->buttons & IN_ATTACK) {
        utils::SetSendPacket(true);
        return;
    }

    const auto moveType = globals::core::localPlayer->GetMoveType();
    if (moveType == MOVETYPE_LADDER || moveType == MOVETYPE_NOCLIP) {
        utils::SetSendPacket(true);
        return;
    }

    const auto oldViewAngles  = cmd->viewAngles;
    const auto oldForwardMove = cmd->forwardMove;
    const auto oldSideMove    = cmd->sideMove;

    static auto shouldDesync = false;
    static auto danceNextTime = interfaces->globalVars->curtime + 2;
    static float spinYaw = 0.f;

    cmd->viewAngles.p = 89.f; // pitch down

    spinYaw += 25.f; // spin speed — tune this
    if (spinYaw > 180.f)
        spinYaw -= 360.f;

    if (shouldDesync) {
        cmd->viewAngles.y = spinYaw + 180.f; // opposite side
        utils::SetSendPacket(false);
    } else {
        cmd->viewAngles.y = spinYaw;
        utils::SetSendPacket(true);
    }

    if (interfaces->globalVars->curtime >= danceNextTime) {
        //interfaces->engineClient->ClientCmd_Unrestricted("_DarkRP_DoAnimation 1617; say gtfo");
        danceNextTime = interfaces->globalVars->curtime + 2;
    }

    shouldDesync = !shouldDesync;
    math::NormalizeAngles(cmd->viewAngles);

    utils::CorrectMovement(oldViewAngles, cmd, oldForwardMove, oldSideMove);
}

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

    bhop->Run(cmd);

    AntiAimRun(cmd);

    static auto fakeLagCounter = 0;
    if (fakeLagCounter <= 14) {
        fakeLagCounter = 0;
    }
    else {
        utils::ChokePacket();
        fakeLagCounter++;
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
