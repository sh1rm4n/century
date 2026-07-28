//
// Created by sh1rm4n on 7/9/26.
//

#include "AntiAim.hpp"

#include "../core/utils.hpp"
#include "../core/globals.hpp"

#include "../sourcesdk/typedefs.hpp"
#include "../sourcesdk/CUserCmd.hpp"

#include "../sourcesdk/entities/C_BaseEntity.hpp"

void AntiAim::Run(CUserCmd* cmd) noexcept {
    if (!globals::Core::localPlayer || !cmd || !globals::Core::localPlayer->IsAlive()) {
        globals::Core::sendPacket = true;
        return;
    }

    if (cmd->buttons & IN_ATTACK) {
        globals::Core::sendPacket = true;
        return;
    }

    const auto moveType = globals::Core::localPlayer->GetMoveType();
    if (moveType == MOVETYPE_LADDER || moveType == MOVETYPE_NOCLIP) {
        globals::Core::sendPacket = true;
        return;
    }

    const auto oldViewAngles  = cmd->viewAngles;
    const auto oldForwardMove = cmd->forwardMove;
    const auto oldSideMove    = cmd->sideMove;

    cmd->viewAngles.p = 89.9f;

    static bool shouldDesync = false;
    static int nextUpdateLby = 0;
    static Angle lastFakeAngle = {};

    const int currentTick = interfaces->globalVars->tickCount;

    if (currentTick >= nextUpdateLby) {
        globals::Hacks::AntiAim::breakingLby = true;
        cmd->viewAngles.y += 120.0f;
        nextUpdateLby = currentTick + TIME_TO_TICKS(1.1f);
    } else {
        globals::Hacks::AntiAim::breakingLby = false;
        if (shouldDesync) {
            cmd->viewAngles.y += 120.0f;
            lastFakeAngle = cmd->viewAngles;
            globals::Core::sendPacket = false;
        } else {
            cmd->viewAngles.y -= 120.0f;
            globals::Core::sendPacket = true;
        }
    }

    shouldDesync = !shouldDesync;
    utils::CorrectMovement(oldViewAngles, cmd, oldForwardMove, oldSideMove);

    if (globals::Core::localPlayer->GetVelocity().Length2D() < 1.0f) {
        if (shouldDesync) {
            cmd->forwardMove += 5.1f;
            cmd->sideMove += 5.1f;
        } else {
            cmd->forwardMove -= 5.1f;
            cmd->sideMove -= 5.1f;
        }
    }

    const auto animState = globals::Core::localPlayer->GetAnimState();
    animState->goal_feet_yaw = lastFakeAngle.y;
}