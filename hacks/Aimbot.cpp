#include "Aimbot.hpp"

#include "Resolver.hpp"
#include "../core/globals.hpp"
#include "../core/interfaces.hpp"
#include "../core/utils.hpp"

#include "../hacks/BackTracking.hpp"

#include "../sourcesdk/CUserCmd.hpp"
#include "../sourcesdk/entities/C_BaseCombatWeapon.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"

void Aimbot::Run(CUserCmd* cmd) noexcept {
    if (!globals::Core::localPlayer || !globals::Core::localPlayer->IsAlive())
        return;

    const auto weapon = globals::Core::localPlayer->GetActiveWeapon();
    if (!weapon)
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

    for (auto i = 1; i <= interfaces->globalVars->maxClients; ++i) {
        const auto player = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

        if (!player || player == globals::Core::localPlayer || player->IsDormant() || !player->IsPlayer() || !player->IsAlive() || player->GetHealth() <= 0)
            continue;

        if (globals::Hacks::Aimbot::teamCheck && player->GetTeamNumber() == globals::Core::localPlayer->GetTeamNumber())
            continue;

        if (player->GetFlags() & FL_GODMODE)
            continue;

        player_info_s info;
        if (!interfaces->engineClient->GetPlayerInfo(i, &info))
            continue;

        if (globals::Core::friends.contains(info.name))
            continue;

        std::array<Matrix3x4, 256> bones = {};
        if (!player->SetupBones(bones.data(), 256, 0x7FF00, interfaces->globalVars->curTime))
            continue;

        Vector bone = {};
        BacktrackRecord record = {};

        auto boneIndex = player->LookupBone("ValveBiped.Bip01_Head1");
        if (boneIndex == -1)
            boneIndex = player->LookupBone("ValveBiped.Bip01_Neck1");
        if (boneIndex == -1)
            boneIndex = player->LookupBone("ValveBiped.HC_Body_Bone");

        utils::Warn("boneIndex: {}", boneIndex);

        if (!globals::Hacks::BackTracking::enabled) {
            bone = bones[boneIndex].GetOrigin();
        }
        else {
            if (!backTracking->GetRecordForTime(i, backTracking->GetBestTime(player), record))
                continue;

            bone = record.bones[boneIndex].GetOrigin();
        }

        const auto aimAngles = math::CalculateAngle(eyePosition, bone);
        const auto fov = math::GetFov(currentViewAngles, aimAngles);

        if (fov < bestFov) {
            if (utils::VisCheck(globals::Core::localPlayer, player, eyePosition, bone, false)) {
                bestFov = fov;
                bestAimAngles = aimAngles;
                bestTarget = player;
                bestRecord = record;
            }
        }
    }

    if (bestTarget) {
        if (globals::Hacks::Aimbot::resolver)
            resolver->Resolve(bestTarget, 3);

        auto deltaAngles = bestAimAngles - currentViewAngles;
        math::NormalizeAngles(deltaAngles);

        auto smoothedAngles = currentViewAngles + (deltaAngles / globals::Hacks::Aimbot::smoothness);
        math::NormalizeAngles(smoothedAngles);

        const auto oldAngles = cmd->viewAngles;
        const auto oldForwardMove = cmd->forwardMove;
        const auto oldSideMove = cmd->sideMove;

        if (!globals::Hacks::Aimbot::silentAim && !globals::Hacks::Aimbot::perfectSilent) {
            cmd->viewAngles = smoothedAngles;
            interfaces->engineClient->SetViewAngles(cmd->viewAngles);
        }

        globals::Hacks::Aimbot::shooting = true;

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

        globals::Hacks::Aimbot::shooting = false;

        utils::CorrectMovement(oldAngles, cmd, oldForwardMove, oldSideMove);
    }
}
