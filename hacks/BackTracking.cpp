//
// Created by sh1rm4n on 7/13/26.
//
#include "BackTracking.hpp"
#include "../core/interfaces.hpp"
#include "../core/globals.hpp"

#include "../sourcesdk/entities/C_BaseEntity.hpp"
#include "../sourcesdk/entities/C_CollisionProperty.hpp"

bool BacktrackRecord::IsValid() {
    const auto netChan = interfaces->engineClient->GetNetChannelInfo();

    static auto sv_maxunlag = interfaces->cvar->FindCommandBase("sv_maxunlag");
    if (!netChan || !sv_maxunlag)
        return false;

    const auto lerpTime = BackTracking::GetLerpTime();

    const auto predictedArrivalTick = TIME_TO_TICKS(netChan->GetLatency(0) + netChan->GetLatency(1)) + (globals::Core::currentCmd->tickCount + 1);

    const auto correct = std::clamp(netChan->GetLatency(0) + BackTracking::GetLerpTime(), 0.f, sv_maxunlag->GetFloat());

    const auto deltaTime = correct - TICKS_TO_TIME(predictedArrivalTick + TIME_TO_TICKS(lerpTime) - (TIME_TO_TICKS(this->simulationTime) + TIME_TO_TICKS(lerpTime)));

    return std::fabs(deltaTime) <= 0.2f;
}

void BackTracking::Update() noexcept {
    for (auto i = 1; i <= interfaces->globalVars->maxClients; i++) {
        const auto player = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);
        if (!player || player == globals::Core::localPlayer || !player->IsPlayer() || !player->IsAlive())
            continue;

        const auto simulationTime = player->GetSimulationTime();

        if (simulationTime == player->GetOldSimulationTime())
            continue;

        std::array<Matrix3x4, 256> bones = {};
        if (!player->SetupBones(bones.data(), 256, 0x7FF00, simulationTime))
            continue;

        BacktrackRecord record{};
        record.simulationTime = simulationTime;
        record.flags = player->GetFlags();
        record.mins = player->GetCollisionProp()->OBBMins();
        record.maxs = player->GetCollisionProp()->OBBMaxs();
        record.bones = bones;
        record.angles = player->GetAbsAngles();
        record.origin = player->GetAbsOrigin();

        if (record.IsValid())
            backtrackRecords[i].emplace_back(record);

        if (backtrackRecords[i].size() > std::clamp(globals::Hacks::BackTracking::ticks, globals::Hacks::BackTracking::ticks, static_cast<int>(1 / interfaces->globalVars->intervalPerTick)))
            backtrackRecords[i].pop_front();
    }
}

float BackTracking::GetLerpTime() noexcept {
    const auto clInterp = interfaces->cvar->FindCommandBase("cl_interp");
    const auto clInterpRatio = interfaces->cvar->FindCommandBase("cl_interp_ratio");
    const auto clUpdateRate = interfaces->cvar->FindCommandBase("cl_updaterate");
    if (!clInterp || !clInterpRatio || !clUpdateRate)
        return 0.0f;
    return std::max(clInterp->GetFloat(), clInterpRatio->GetFloat() / clUpdateRate->GetFloat());
}

bool BackTracking::GetRecordForTime(int playerIndex, float targetTime, BacktrackRecord& out) noexcept {
    auto& records = backtrackRecords[playerIndex];
    if (records.empty())
        return false;

    auto it = std::lower_bound(records.begin(), records.end(), targetTime,
        [](const BacktrackRecord& rec, float t) { return rec.simulationTime < t; });

    if (it == records.begin()) {
        out = *it;
        return true;
    }
    if (it == records.end()) {
        out = records.back();
        return true;
    }

    const auto prev = std::prev(it);
    out = (std::fabs(it->simulationTime - targetTime) < std::fabs(prev->simulationTime - targetTime))
        ? *it : *prev;
    return true;
}

void BackTracking::Apply(CUserCmd* cmd, C_BaseEntity *player, const BacktrackRecord &record) {
    if (!cmd || !player)
        return;

    if (player->GetVelocity().Length2D() < 49.9f)
        return;

    player->GetSimulationTime() = record.simulationTime;
    player->GetFlags() = record.flags;
    player->GetCollisionProp()->OBBMins() = record.mins;
    player->GetCollisionProp()->OBBMaxs() = record.maxs;
    player->SetAbsAngles(record.angles);
    player->SetAbsOrigin(record.origin);
}