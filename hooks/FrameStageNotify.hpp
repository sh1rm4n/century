//
// Created by sh1rm4n on 5/26/26.
//

#ifndef CENTURY_FRAMESTAGENOTIFY_HPP
#define CENTURY_FRAMESTAGENOTIFY_HPP

#include <polyhook2/Detour/x64Detour.hpp>

#include "../sourcesdk/interfaces/HLClient.hpp"
#include "../sourcesdk/interfaces/CEngineClient.hpp"

#include "../sourcesdk/entities/C_BaseEntity.hpp"

#include "../core/globals.hpp"
#include "../sourcesdk/C_MultiPlayerAnimState.hpp"

inline std::uint64_t ogFrameStageNotify = 0;

void dtFrameStageNotify(HLClient* vmt, ClientFrameStage_t stage, void* unk1, void* unk2) {
    static auto ogFunction = PLH::FnCast(ogFrameStageNotify, dtFrameStageNotify);

    if (!interfaces->engineClient->IsInGame() || !globals::core::localPlayer)
        return ogFunction(vmt, stage, unk1, unk2);

    ogFunction(vmt, stage, unk1, unk2);

    switch (stage) {
        default: break;

        case FRAME_NET_UPDATE_END: {
            if (interfaces->engineClient->IsInGame()) {

            }
        } break;

        case FRAME_RENDER_START: {
            globals::core::localPlayer->SetLocalViewAngles(globals::core::currentCmd->viewAngles);

            for (auto i = 0; i <= interfaces->globalVars->maxClients; i++) {
                const auto entity = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

                if (!entity || entity == globals::core::localPlayer || !entity->IsPlayer() || !entity->IsAlive())
                    continue;
            }
        } break;
    }
}

#endif //CENTURY_FRAMESTAGENOTIFY_HPP
