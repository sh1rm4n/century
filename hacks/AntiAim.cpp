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
        utils::SetSendPacket(true);
        return;
    }

    // always send on attack
    if (cmd->buttons & IN_ATTACK) {
        utils::SetSendPacket(true);
        return;
    }

    const auto moveType = globals::Core::localPlayer->GetMoveType();
    if (moveType == MOVETYPE_LADDER || moveType == MOVETYPE_NOCLIP) {
        utils::SetSendPacket(true);
        return;
    }

    const auto oldViewAngles  = cmd->viewAngles;
    const auto oldForwardMove = cmd->forwardMove;
    const auto oldSideMove    = cmd->sideMove;

    static bool  shouldDesync = false;
    static int   jitterIndex  = 0;

    // jitter table — predefined offsets cycle each tick
    static constexpr float jitterYaw[] = {
        90.f, -90.f, 135.f, -135.f, 180.f, 45.f, -45.f, 120.f
    };
    static constexpr float jitterPitch[] = {
        89.f, -89.f, 45.f, -45.f, 89.f, 0.f, -89.f, 45.f
    };
    static constexpr int JITTER_COUNT = 8;

    const float yawOffset   = jitterYaw[jitterIndex % JITTER_COUNT];
    const float pitchOffset = jitterPitch[jitterIndex % JITTER_COUNT];

    if (shouldDesync) {
        cmd->viewAngles.p += pitchOffset;
        cmd->viewAngles.y += yawOffset;
        utils::SetSendPacket(false); // choke — server stuck on last real angle
    } else {
        cmd->viewAngles.p -= pitchOffset;
        cmd->viewAngles.y -= yawOffset;
        utils::SetSendPacket(true);  // send — update server position
    }

    jitterIndex++;
    shouldDesync = !shouldDesync;
    math::NormalizeAngles(cmd->viewAngles);

    utils::CorrectMovement(oldViewAngles, cmd, oldForwardMove, oldSideMove);

    if (globals::Core::localPlayer->GetVelocity().Length2D() < 1.f) {
        if (shouldDesync) {
            cmd->forwardMove += 35;
            cmd->sideMove += 35;
        }
        else {
            cmd->forwardMove -= 35;
            cmd->sideMove -= 35;
        }
    }
}