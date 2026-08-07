//
// Created by sh1rm4n on 5/26/26.
//

#ifndef CENTURY_FRAMESTAGENOTIFY_HPP
#define CENTURY_FRAMESTAGENOTIFY_HPP

#pragma once

#include <polyhook2/Detour/x64Detour.hpp>

#include "FireBullets.hpp"
#include "../sourcesdk/interfaces/HLClient.hpp"
#include "../sourcesdk/interfaces/CEngineClient.hpp"

#include "../sourcesdk/entities/C_BaseEntity.hpp"

#include "../core/globals.hpp"

#include "../hacks/ForwardTracking.hpp"
#include "../hacks/BackTracking.hpp"
#include "../hacks/NameStealer.hpp"

inline std::uint64_t ogFrameStageNotify = 0;

void dtFrameStageNotify(HLClient* vmt, ClientFrameStage_t stage, void* unk1, void* unk2) {
    static auto ogFunction = PLH::FnCast(ogFrameStageNotify, dtFrameStageNotify);

    if (!interfaces->engineClient->IsInGame() || !globals::Core::localPlayer)
        return ogFunction(vmt, stage, unk1, unk2);

    ogFunction(vmt, stage, unk1, unk2);

    switch (stage) {
        default: break;

        case FRAME_NET_UPDATE_END: {
            backTracking->Update();
        } break;

        case FRAME_RENDER_START: {
            if (!hkFireBullets) {
                hkFireBullets = std::make_unique<PLH::x64Detour>(
                    memory::GetVMTFunc<std::uint64_t>(globals::Core::localPlayer, FIREBULLETS_INDEX),
                    reinterpret_cast<std::uint64_t>(dtFireBullets), &ogFireBullets);

                if (hkFireBullets->hook())
                    utils::Warn("successfully hooked firebullets");
            }

            for (auto i = 1; i <= interfaces->globalVars->maxClients; i++) {
                const auto player = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

                if (!player || player == globals::Core::localPlayer || player->IsDormant() || !player->IsPlayer() || !player->IsAlive())
                    continue;

                player_info_s info{};
                if (!interfaces->engineClient->GetPlayerInfo(i, &info))
                    continue;

                if (!nameStealer->IsNameAdded(info.name))
                    nameStealer->AddName(info.name);

                //nameStealer->Run();

                const int nCmdMaxTickDelta = ( 1.f / interfaces->globalVars->intervalPerTick ) * 2.5f;
                const int nMinDelta = std::max( 0, interfaces->globalVars->tickCount - nCmdMaxTickDelta );
                const int nMaxDelta = interfaces->globalVars->tickCount + nCmdMaxTickDelta;

                player->GetClientSideAnimation() = false;

                utils::InvalidateBoneCache();
                utils::UpdateClientSideAnimations();

                player->GetClientSideAnimation() = true;

                if (globals::Hacks::ForwardTracking::enabled)
                    forwardTracking->Run(globals::Core::currentCmd, player);

                if (globals::Hacks::BackTracking::enabled) {
                    BacktrackRecord record{};
                    backTracking->GetRecordForTime(i, TICKS_TO_TIME(globals::Core::currentCmd->tickCount - 8), record);

                    /*backTracking->Apply(
                        globals::Core::currentCmd,
                        player,
                        record);*/
                    globals::Core::currentCmd->tickCount -= 8;
                }
            }
        } break;
    }
}

#endif //CENTURY_FRAMESTAGENOTIFY_HPP
