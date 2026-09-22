//
// Created by sh1rm4n on 7/9/26.
//

#include "AntiAim.hpp"

#include "../core/utils.hpp"
#include "../core/globals.hpp"

#include "../sourcesdk/typedefs.hpp"
#include "../sourcesdk/CUserCmd.hpp"

#include "../sourcesdk/entities/C_BaseEntity.hpp"

void LBYBreak(CUserCmd* cmd) {
    if (!globals::Core::localPlayer || !cmd || !globals::Core::localPlayer->IsAlive())
        return;

    static auto nextUpdate = 0;

    const auto animState = globals::Core::localPlayer->GetAnimState();

    if (interfaces->globalVars->tickCount >= nextUpdate) {
        //globals::Core::sendPacket             = false;
        globals::Hacks::AntiAim::breakingLby  = false;

        cmd->viewAngles.y += animState->goal_feet_yaw + 180.f;
        //math::NormalizeAngles(cmd->viewAngles);

        const auto isMoving = globals::Core::localPlayer->GetVelocity().Length2D() >= 1.f;

        nextUpdate = interfaces->globalVars->tickCount + TIME_TO_TICKS(isMoving ? 0.1f : 1.1f);
    } else {
        //globals::Core::sendPacket            = true;
        globals::Hacks::AntiAim::breakingLby = true;

        cmd->viewAngles.y += animState->current_feet_yaw - 45.f;
        //math::NormalizeAngles(cmd->viewAngles);
    }

    utils::Warn("lby yaw: {}", animState->goal_feet_yaw );
}

void AntiAim::Run(CUserCmd* cmd) noexcept {
    if (!globals::Core::localPlayer || !cmd || !globals::Core::localPlayer->IsAlive()) {
        globals::Core::sendPacket = true;
        return;
    }

    if (cmd->buttons & IN_ATTACK) {
        if (!globals::Hacks::Aimbot::shooting) {
            globals::Core::sendPacket = true;
            return;
        }
    }

    const auto moveType = globals::Core::localPlayer->GetMoveType();
    if (moveType == MOVETYPE_LADDER || moveType == MOVETYPE_NOCLIP) {
        globals::Core::sendPacket = true;
        return;
    }

    const auto oldViewAngles  = cmd->viewAngles;
    const auto oldForwardMove = cmd->forwardMove;
    const auto oldSideMove    = cmd->sideMove;

    //static bool shouldDesync = false;

    cmd->viewAngles.p = 89.f;

    LBYBreak(cmd);

    const auto randomizedSide = utils::Random(1.f, 4.f);

    switch (cmd->commandNumber % 4) {
        case 0:
            cmd->viewAngles.y = 45.9f * randomizedSide;
            globals::Core::sendPacket = false;
            break;
        case 1:
            cmd->viewAngles.y = 90.9f * randomizedSide;
            globals::Core::sendPacket = true;
            break;
        case 2:
            cmd->viewAngles.y = -90.9f * randomizedSide;
            globals::Core::sendPacket = false;
            break;
        case 3:
            cmd->viewAngles.y = -45.9f * randomizedSide;
            globals::Core::sendPacket = true;
            break;
    }
    
    //cmd->viewAngles.y = 135.f;

    //shouldDesync = !shouldDesync;

    //if (!globals::Hacks::FakeLag::enabled)
    //    globals::Core::sendPacket = shouldDesync;

    utils::CorrectMovement(oldViewAngles, cmd, oldForwardMove, oldSideMove);

    if (globals::Core::localPlayer->GetVelocity().Length2D() < 1.0f) {
        if (cmd->commandNumber % 2 == 0) {
            cmd->sideMove += 8.f;
        } else {
            cmd->sideMove -= 8.f;
        }
    }
}