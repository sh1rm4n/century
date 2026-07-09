#include "Aimbot.hpp"

#include "../core/globals.hpp"
#include "../core/interfaces.hpp"
#include "../core/utils.hpp"

#include "../sourcesdk/UserCmd.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"

void Aimbot::Run(UserCmd* cmd) noexcept {
    if (!globals::core::localPlayer || !globals::core::localPlayer->IsAlive())
        return;

    float bestFov = globals::hacks::aimbot::fov;
    float finalDistance = FLT_MAX;

    Angle bestAimAngles = {};
    C_BaseEntity* bestTarget = nullptr;

    const auto eyePosition = globals::core::localPlayer->GetEyePosition();
    const auto origin = globals::core::localPlayer->GetAbsOrigin();

    Angle currentViewAngles;
    interfaces->engineClient->GetViewAngles(currentViewAngles);

    for (int i = 0; i <= interfaces->globalVars->maxClients; i++) {
        const auto player = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

        if (!player || player == globals::core::localPlayer || !player->IsPlayer() || !player->IsAlive() || player->GetHealth() <= 0)
            continue;

        if (globals::hacks::aimbot::teamCheck && player->GetTeamNumber() == globals::core::localPlayer->GetTeamNumber())
            continue;

        player_info_s info;
        if (!interfaces->engineClient->GetPlayerInfo(i, &info))
            continue;

        if (globals::core::friends.contains(info.name))
            continue;

        Matrix3x4 bones[256];
        if (!player->SetupBones(bones, 256, 0x7FF00, interfaces->globalVars->curtime))
            continue;

        const auto bone = bones[7].GetOrigin();
        const auto aimAngles = math::CalculateAngle(eyePosition, bone);
        const auto fov = math::GetFov(currentViewAngles, aimAngles);

        if (fov < bestFov) {
            if (utils::VisCheck(globals::core::localPlayer, player, eyePosition, bone)) {
                bestFov = fov;
                bestAimAngles = aimAngles;
                bestTarget = player;
            }
        }
    }

    if (bestTarget) {
        Angle deltaAngles = bestAimAngles - currentViewAngles;
        math::NormalizeAngles(deltaAngles);

        Angle smoothedAngles = currentViewAngles + (deltaAngles / globals::hacks::aimbot::smoothness);
        math::NormalizeAngles(smoothedAngles);

        const auto oldAngles = cmd->viewAngles;
        const auto oldForwardMove = cmd->forwardMove;
        const auto oldSideMove = cmd->sideMove;

        if (!globals::hacks::aimbot::silentAim && !globals::hacks::aimbot::perfectSilent) {
            cmd->viewAngles = smoothedAngles;
            interfaces->engineClient->SetViewAngles(cmd->viewAngles);
        }

        if (cmd->commandNumber % 2 == 0) {
            if (globals::hacks::aimbot::silentAim) {
                cmd->viewAngles = smoothedAngles;
            }
            else if (globals::hacks::aimbot::perfectSilent) {
                cmd->viewAngles = smoothedAngles;
                utils::SetSendPacket(false);
            }

            cmd->buttons |= IN_ATTACK;
        }
        else {
            if (globals::hacks::aimbot::perfectSilent) {
                cmd->viewAngles = oldAngles;
                utils::SetSendPacket(true);
            }

            cmd->buttons &= ~IN_ATTACK;
        }

        utils::CorrectMovement(oldAngles, cmd, oldForwardMove, oldSideMove);
    }
}
