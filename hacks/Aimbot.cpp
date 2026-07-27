#include "Aimbot.hpp"

#include "BackTracking.hpp"
#include "Bruteforce.hpp"
#include "ForwardTracking.hpp"

#include "../core/globals.hpp"
#include "../core/interfaces.hpp"
#include "../core/utils.hpp"

#include "../sourcesdk/CUserCmd.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"

void Aimbot::Run(CUserCmd* cmd) noexcept {
    if (!globals::Core::localPlayer || !globals::Core::localPlayer->IsAlive())
        return;

    float bestFov = globals::Hacks::Aimbot::fov;
    float finalDistance = FLT_MAX;

    Angle bestAimAngles = {};
    C_BaseEntity* bestTarget = nullptr;
    BacktrackRecord bestRecord = {};

    const auto eyePosition = globals::Core::localPlayer->GetEyePosition();
    const auto origin = globals::Core::localPlayer->GetNetworkedOrigin();

    Angle currentViewAngles;
    interfaces->engineClient->GetViewAngles(currentViewAngles);

    for (int i = 0; i <= interfaces->globalVars->maxClients; i++) {
        const auto player = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

        if (!player || player == globals::Core::localPlayer || !player->IsPlayer() || !player->IsAlive() || player->GetHealth() <= 0)
            continue;

        if (globals::Hacks::Aimbot::teamCheck && player->GetTeamNumber() == globals::Core::localPlayer->GetTeamNumber())
            continue;

        player_info_s info;
        if (!interfaces->engineClient->GetPlayerInfo(i, &info))
            continue;

        if (globals::Core::friends.contains(info.name))
            continue;

        if (globals::Hacks::ForwardTracking::enabled)
            forwardTracking->Run(cmd, player);

        std::array<Matrix3x4, 256> bones = {};

        BacktrackRecord record{};

        if (globals::Hacks::BackTracking::enabled) {
            backTracking->GetRecordForTime(i, player->GetSimulationTime() - TICKS_TO_TIME(globals::Hacks::BackTracking::ticks), record);
            utils::Warn("time target: {}", TIME_TO_TICKS(player->GetSimulationTime() - TICKS_TO_TIME(globals::Hacks::BackTracking::ticks)));

            bones = record.bones;
        }
        else {
            if (!player->SetupBones(bones.data(), 256, 0x7FF00, interfaces->globalVars->curTime))
                continue;
        }

        //auto bone = bones[7].GetOrigin();
        auto bone = bones[player->LookupBone("ValveBiped.Bip01_Head1")].GetOrigin();

        if (globals::Hacks::Aimbot::bruteforce)
            bone = bruteForcer->Brute(player, bone);

        const auto aimAngles = math::CalculateAngle(eyePosition, bone);
        const auto fov = math::GetFov(currentViewAngles, aimAngles);

        if (fov < bestFov) {
            if (utils::VisCheck(globals::Core::localPlayer, player, eyePosition, bone)) {
                bestFov = fov;
                bestAimAngles = aimAngles;
                bestTarget = player;
                bestRecord = record;
            }
        }
    }

    if (bestTarget) {
        if (globals::Hacks::BackTracking::enabled)
            backTracking->Apply(cmd, bestTarget, bestRecord);

        Angle deltaAngles = bestAimAngles - currentViewAngles;
        math::NormalizeAngles(deltaAngles);

        Angle smoothedAngles = currentViewAngles + (deltaAngles / globals::Hacks::Aimbot::smoothness);
        math::NormalizeAngles(smoothedAngles);

        const auto oldAngles = cmd->viewAngles;
        const auto oldForwardMove = cmd->forwardMove;
        const auto oldSideMove = cmd->sideMove;

        if (!globals::Hacks::Aimbot::silentAim && !globals::Hacks::Aimbot::perfectSilent) {
            cmd->viewAngles = smoothedAngles;
            interfaces->engineClient->SetViewAngles(cmd->viewAngles);
        }

        if (cmd->commandNumber % 2 == 0) {
            if (globals::Hacks::Aimbot::silentAim) {
                cmd->viewAngles = smoothedAngles;
            }
            else if (globals::Hacks::Aimbot::perfectSilent) {
                cmd->viewAngles = smoothedAngles;
                globals::Core::sendPacket = false;
            }

            cmd->buttons |= IN_ATTACK;
        }
        else {
            if (globals::Hacks::Aimbot::perfectSilent) {
                cmd->viewAngles = oldAngles;
                globals::Core::sendPacket = true;
            }

            cmd->buttons &= ~IN_ATTACK;
        }

        utils::CorrectMovement(oldAngles, cmd, oldForwardMove, oldSideMove);
    }
}
