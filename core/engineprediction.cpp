//
// Created by sh1rm4n on 6/16/26.
//

#include "engineprediction.hpp"

#include <cstring>

#include "globals.hpp"
#include "interfaces.hpp"
#include "utils.hpp"

#include "../sourcesdk/entities/C_BaseEntity.hpp"

#include "../3rd/checksum_md5.h"

void EnginePrediction::Begin(UserCmd* cmd) noexcept {
    if (!globals::core::localPlayer)
        return;

    utils::SetPredictionSeed(MD5_PseudoRandom(cmd->randomSeed) & 0x7FFFFFFF);

    oldCurTime = interfaces->globalVars->curtime;
    oldFrameTime = interfaces->globalVars->frametime;

    interfaces->globalVars->curtime = globals::core::localPlayer->GetSimulationTime() * interfaces->globalVars->interval_per_tick;
    interfaces->globalVars->frametime = interfaces->globalVars->interval_per_tick;

    interfaces->gameMovement->StartTrackPredictionErrors(globals::core::localPlayer);

    CMoveData moveData;
    std::memset(&moveData, 0, sizeof(moveData));

    interfaces->prediction->SetupMove(globals::core::localPlayer, cmd, interfaces->moveHelper, &moveData);
    interfaces->gameMovement->ProcessMovement(globals::core::localPlayer, &moveData);
    interfaces->prediction->FinishMove(globals::core::localPlayer, cmd, &moveData);
}

void EnginePrediction::End() noexcept {
    interfaces->gameMovement->FinishTrackPredictionErrors(globals::core::localPlayer);

    utils::SetPredictionSeed(-1);

    interfaces->globalVars->curtime = oldCurTime;
    interfaces->globalVars->frametime = oldFrameTime;
}