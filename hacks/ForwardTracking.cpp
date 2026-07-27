//
// Created by sh1rm4n on 7/13/26.
//

#include "ForwardTracking.hpp"

#include "../core/globals.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"

void ForwardTracking::Run(CUserCmd* cmd, C_BaseEntity* player) noexcept {
    if (player->GetVelocity().Length2D() < 99.9f)
        return;

    const auto cmdHighestPossible = interfaces->globalVars->tickCount + ((1.f / interfaces->globalVars->intervalPerTick) * 2.5f);

    cmd->tickCount = cmdHighestPossible - 1;

    utils::InvalidateBoneCache();

    //player->SetAbsOrigin(player->GetNetworkedOrigin());
    //player->SetAbsAngles(player->GetNetworkedAngles());
    player->SetAbsOrigin(player->GetAbsOrigin() + (player->GetVelocity() * (interfaces->globalVars->intervalPerTick * globals::Hacks::ForwardTracking::ghettoPredictionAmount)));
}
