//
// Created by sh1rm4n on 7/13/26.
//

#ifndef CENTURY_BACKTRACKING_HPP
#define CENTURY_BACKTRACKING_HPP

#include <deque>
#include <memory>
#include <unordered_map>

#include "../sourcesdk/math.hpp"

class CUserCmd;
class C_BaseEntity;

struct BacktrackRecord {
    double simulationTime{};
    int tickCount{};
    int flags{};
    Vector mins{};
    Vector maxs{};
    std::array<Matrix3x4, 256> bones;
    Angle angles{};
    Vector origin{};

    bool IsValid();
};

class BackTracking {
private:
    std::unordered_map<int, std::deque<BacktrackRecord>> backtrackRecords;
public:
    void Update() noexcept;
    static float GetLerpTime() noexcept;
    bool GetRecordForTime(int playerIndex, double targetTime, BacktrackRecord& out) noexcept;
    void Apply(CUserCmd* cmd, C_BaseEntity* player, BacktrackRecord& record);
    double GetBestTime(C_BaseEntity* player) noexcept;
    bool IsBreakingLC(C_BaseEntity* entity) noexcept;
    std::deque<BacktrackRecord> GetRecords(C_BaseEntity* entity) noexcept;
};

inline auto backTracking = std::make_unique<BackTracking>();

#endif //CENTURY_BACKTRACKING_HPP
