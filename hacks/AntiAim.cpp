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
    static Angle lastFakeAngle = {};

    static std::array<float, 8> jitterYaws = {135.f, -135.f, 45.f, -45.f, 90.f, -90.f, 180.f, -180.f};

    if (shouldDesync) {
        cmd->viewAngles.y += jitterYaws[utils::Random(0, 8)];
        lastFakeAngle = cmd->viewAngles;
    }
    else {
        cmd->viewAngles.y += jitterYaws[utils::Random(0, 8)];
    }

    shouldDesync = !shouldDesync;

    if (!globals::Hacks::FakeLag::enabled)
        globals::Core::sendPacket = shouldDesync;

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