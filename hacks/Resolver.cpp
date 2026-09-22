//
// Created by sh1rm4n on 7/9/26.
//

#include "Resolver.hpp"

#include "../core/globals.hpp"
#include "../core/interfaces.hpp"
#include "../core/utils.hpp"

#include "../sourcesdk/math.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"
#include "../sourcesdk/interfaces/CGlobalVars.hpp"

void Resolver::Resolve(C_BaseEntity* player, int shotsMade) noexcept {
    if (!player || !player->IsAlive() || player->IsDormant())
        return;

    if (shotsMade <= 2)
        return;

    if (!resolvingPlayers.contains(player))
        resolvingPlayers.emplace(player);

    utils::Warn("shotsMade: {}" , shotsMade);

    player->InvalidateBoneCache();

    const auto yaw = utils::Random(-100.0f, 100.0f);

    /*auto angles = player->GetAbsAngles();

    angles.y += yaw;

    player->SetAbsAngles(angles);
    player->GetAbsAngles() = angles;
    player->SetLocalViewAngles(angles);
    player->GetEyeAngles() = angles;*/

    player->GetPoseParameters()[11] = yaw * 0.25f;

    //player->SetPoseParameter(player->GetStudioHdr(), "body_yawww", yaw);

    utils::Warn("pose parameters: 0x{:x}" , reinterpret_cast<std::uint64_t>(player->GetPoseParameters()));

    Matrix3x4 matrix[256];
    player->SetupBones(matrix, 256, 0x7FF00, interfaces->globalVars->curTime);

    player->UpdateClientSideAnimation();
}

void Resolver::RemovePlayer(C_BaseEntity* player) noexcept {
    if (!player)
        return;

    if (resolvingPlayers.contains(player))
        resolvingPlayers.erase(player);
}

bool Resolver::ContainsPlayer(C_BaseEntity *player) noexcept {
    if (!player)
        return false;

    return resolvingPlayers.contains(player);
}
