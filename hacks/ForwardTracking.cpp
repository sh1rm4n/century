//
// Created by sh1rm4n on 7/13/26.
//

#include "ForwardTracking.hpp"

#include "../core/globals.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"

void ForwardTracking::Run(CUserCmd* cmd, C_BaseEntity* player) noexcept {
    const auto cmdHighestPossible = static_cast<int>(interfaces->globalVars->tickCount + ((1.f / interfaces->globalVars->intervalPerTick) * 2.5f));

    //utils::InvalidateBoneCache();

    player->SetAbsOrigin(player->GetNetworkedOrigin());
    player->SetAbsAngles(player->GetNetworkedAngles());
    cmd->tickCount = cmdHighestPossible - 1;
}
