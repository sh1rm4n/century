//
// Created by sh1rm4n on 7/9/26.
//

#include "AntiAim.hpp"

#include "../core/utils.hpp"
#include "../core/globals.hpp"

#include "../sourcesdk/typedefs.hpp"
#include "../sourcesdk/UserCmd.hpp"

#include "../sourcesdk/entities/C_BaseEntity.hpp"

void AntiAim::Run(UserCmd* cmd) noexcept {
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
        cmd->viewAngles.y += 90.f; // opposite side
        utils::SetSendPacket(false);
    } else {
        cmd->viewAngles.y += -90.f;
        utils::SetSendPacket(true);
    }

    if (interfaces->globalVars->curtime >= danceNextTime) {
        //interfaces->engineClient->ClientCmd_Unrestricted("_DarkRP_DoAnimation 1617; say gtfo");
        danceNextTime = interfaces->globalVars->curtime + 2;
    }

    shouldDesync = !shouldDesync;
    math::NormalizeAngles(cmd->viewAngles);

    utils::CorrectMovement(oldViewAngles, cmd, oldForwardMove, oldSideMove);

    if (globals::core::localPlayer->GetVelocity().Length2D() < 1.f) {
        if (shouldDesync) {
            cmd->forwardMove += 10;
            cmd->sideMove += 10;
        }
        else {
            cmd->forwardMove -= 10;
            cmd->sideMove -= 10;
        }
    }
}