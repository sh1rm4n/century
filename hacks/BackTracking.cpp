//
// Created by sh1rm4n on 7/13/26.
//
#include "BackTracking.hpp"
#include "../core/interfaces.hpp"
#include "../core/globals.hpp"

#include "../sourcesdk/entities/IClientUnknown.hpp"
#include "../sourcesdk/entities/IClientRenderable.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"
#include "../sourcesdk/entities/C_CollisionProperty.hpp"

bool BacktrackRecord::IsValid() {
    const auto netChan = interfaces->engineClient->GetNetChannelInfo();
    if (!netChan)
        return false;

    static auto sv_maxunlag = interfaces->cvar->FindCommandBase("sv_maxunlag");
    if (!sv_maxunlag)
        return false;

    const auto correct = std::clamp<float>(netChan->GetLatency(0) + netChan->GetLatency(1), 0, sv_maxunlag->GetFloat());

    utils::Warn("correct: {}", correct);

    return std::abs(correct - (interfaces->globalVars->curTime - this->simulationTime)) <= 0.2f;
}

void BackTracking::Update() noexcept {
    for (auto i = 1; i <= interfaces->globalVars->maxClients; ++i) {
        const auto player = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);
        if (!player || player->IsDormant() || !player->IsPlayer() || !player->IsAlive())
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
        record.mins = player->GetCollideable()->OBBMins();
        record.maxs = player->GetCollideable()->OBBMaxs();
        record.bones = bones;
        record.angles = player->GetNetworkedAngles();
        record.origin = player->GetNetworkedOrigin();

        //if (!record.IsValid())
        //    continue;

        backtrackRecords[i].emplace_back(record);

        if (backtrackRecords[i].size() > static_cast<int>(1 / interfaces->globalVars->intervalPerTick))
            backtrackRecords[i].pop_front();
    }
}

float BackTracking::GetLerpTime() noexcept {
    static auto clInterp = interfaces->cvar->FindCommandBase("cl_interp");
    static auto clInterpRatio = interfaces->cvar->FindCommandBase("cl_interp_ratio");
    static auto clUpdateRate = interfaces->cvar->FindCommandBase("cl_updaterate");

    if (!clInterp || !clInterpRatio || !clUpdateRate)
        return 0.0f;

    return std::max(clInterp->GetFloat(), clInterpRatio->GetFloat() / clUpdateRate->GetFloat());
}

bool BackTracking::GetRecordForTime(int playerIndex, double targetTime, BacktrackRecord& out) noexcept {
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

void BackTracking::Apply(CUserCmd* cmd, C_BaseEntity *player, BacktrackRecord &record) {
    if (!cmd || !player)
        return;

    //cmd->tickCount = TIME_TO_TICKS(record.simulationTime + GetLerpTime()) - globals::Hacks::BackTracking::ticks;
    cmd->tickCount = TIME_TO_TICKS(GetBestTime(player));
}

double BackTracking::GetBestTime(C_BaseEntity* player) noexcept {
    //return (player->GetSimulationTime() + GetLerpTime()) - TICKS_TO_TIME(globals::Hacks::BackTracking::ticks);

    const auto netChan = interfaces->engineClient->GetNetChannelInfo();

    static auto sv_maxunlag = interfaces->cvar->FindCommandBase("sv_maxunlag");
    if (!netChan || !sv_maxunlag)
        return false;

    float correct = 0.f;

    correct += netChan->GetLatency(0) + netChan->GetLatency(1);

    int lerpTicks = TIME_TO_TICKS(BackTracking::GetLerpTime());

    correct += lerpTicks;

    correct = std::clamp(correct, 0.f, sv_maxunlag->GetFloat());

    int targetTick = globals::Core::currentCmd->tickCount - lerpTicks;

    float deltaTime = correct - TICKS_TO_TIME(interfaces->globalVars->tickCount - targetTick);

    if (std::fabs(deltaTime) > 0.2f) {
        targetTick = globals::Core::currentCmd->tickCount - TIME_TO_TICKS(lerpTicks);
    }

    return TICKS_TO_TIME(targetTick - globals::Hacks::BackTracking::ticks);
}

bool BackTracking::IsBreakingLC(C_BaseEntity *entity) noexcept {
    if (!entity || !entity->IsAlive() || entity->IsDormant())
        return false;

    BacktrackRecord record{};
    if (!GetRecordForTime(entity->EntIndex(), GetBestTime(entity), record))
        return false;

    const auto latestOrigin = entity->GetNetworkedOrigin();
    const auto backtrackedOrigin = record.origin;

    return latestOrigin.DistToSqr(backtrackedOrigin) >= 512.f;
}

std::deque<BacktrackRecord> BackTracking::GetRecords(C_BaseEntity *entity) noexcept {
    return backtrackRecords[entity->EntIndex()];
}
