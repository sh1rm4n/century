//
// Created by sh1rm4n on 6/9/26.
//

#include "Aimbot.hpp"

#include "../core/globals.hpp"
#include "../core/interfaces.hpp"
#include "../core/utils.hpp"

#include "../sourcesdk/entities/C_BaseEntity.hpp"

void Aimbot::Run(UserCmd* cmd) noexcept {
    if (!globals::core::localPlayer || !globals::core::localPlayer->IsAlive())
        return;

    constexpr float smoothFactor = 1.0f;
    float bestFov = 10.0f;
    Angle bestAimAngles = {};
    C_BaseEntity* bestTarget = nullptr; // This acts as our tracking flag

    const auto eyePosition = globals::core::localPlayer->GetEyePosition();

    Angle currentViewAngles;
    interfaces->engineClient->GetViewAngles(currentViewAngles);

    // Player indices in Source Engine start at 1 up to maxClients
    for (int i = 0; i <= interfaces->globalVars->maxClients; i++) {
        const auto player = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

        // Explicit health check prevents targeting transitioning ragdolls/spectators
        if (!player || player == globals::core::localPlayer || !player->IsPlayer() || !player->IsAlive() || player->GetHealth() <= 0)
            continue;

        Matrix3x4 bones[256];
        if (!player->SetupBones(bones, 256, 0x7FF00, interfaces->globalVars->curtime))
            continue;

        const auto bone = bones[7].GetOrigin();

        //if (!utils::VisCheck(eyePosition, bone))
        //    continue;

        const auto aimAngles = math::CalculateAngle(eyePosition, bone);
        const auto fov = math::GetFov(currentViewAngles, aimAngles);

        if (fov < bestFov) {
            bestFov = fov;
            bestAimAngles = aimAngles;
            bestTarget = player; // Target locked
        }
    }

    // If bestTarget is still valid, we have a confirmed match
    if (bestTarget) {
        Angle deltaAngles = bestAimAngles - currentViewAngles;
        math::NormalizeAngles(deltaAngles);

        Angle smoothedAngles = currentViewAngles + (deltaAngles / smoothFactor);
        math::NormalizeAngles(smoothedAngles);

        const auto oldAngles = cmd->viewAngles;
        const auto oldForwardMove = cmd->forwardMove;
        const auto oldSideMove = cmd->sideMove;

        cmd->viewAngles = smoothedAngles;
        //interfaces->engineClient->SetViewAngles(smoothedAngles);

        utils::CorrectMovement(oldAngles, cmd, oldForwardMove, oldSideMove);

        if (cmd->commandNumber % 2 == 0) {
            cmd->buttons |= IN_ATTACK;  // Tick 1: Press
        } else {
            cmd->buttons &= ~IN_ATTACK; // Tick 2: Unpress
        }
    }
}