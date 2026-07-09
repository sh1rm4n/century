//
// Created by sh1rm4n on 6/16/26.
//

#ifndef CENTURY_CGAMEMOVEMENT_HPP
#define CENTURY_CGAMEMOVEMENT_HPP

#include "../../core/memory.hpp"

class C_BaseEntity;

class CMoveData {
public:
    bool m_bFirstRunOfFunctions : 1; //0x0000
    bool m_bGameCodeMovedPlayer : 1; //0x0000
    std::uint32_t m_nPlayerHandle; //0x0004
    std::int32_t m_nImpulseCommand; //0x0008
    Angle m_vecViewAngles; //0x000C
    Angle m_vecAbsViewAngles; //0x0018
    std::int32_t m_nButtons; //0x0024
    std::int32_t m_nOldButtons; //0x0028
    float m_flForwardMove; //0x002C
    float m_flSideMove; //0x0030
    float m_flUpMove; //0x0034
    float m_flMaxSpeed; //0x0038
    float m_flClientMaxSpeed; //0x003C
    Vector m_vecVelocity; //0x0040
    Angle m_vecAngles; //0x004C
    Angle m_vecOldAngles; //0x0058
    float m_outStepHeight; //0x0064
    Vector m_outWishVel; //0x0068
    Vector m_outJumpVel; //0x0074
    Vector m_vecConstraintCenter; //0x0080
    float m_flConstraintRadius; //0x008C
    float m_flConstraintWidth; //0x0090
    float m_flConstraintSpeedFactor; //0x0094
    Vector m_vecAbsOrigin; //0x0098
    char pad_0001[8]; //0x00A4
};

class CGameMovement {
public:
    constexpr void StartTrackPredictionErrors(C_BaseEntity* entity) noexcept {
        memory::Call<void>(this, 3, entity);
    }

    constexpr void ProcessMovement(C_BaseEntity* entity, CMoveData* moveData) noexcept {
        memory::Call<void>(this, 5, entity, moveData);
    }

    constexpr void FinishTrackPredictionErrors(C_BaseEntity* entity) noexcept {
        memory::Call<void>(this, 4, entity);
    }
};

#endif //CENTURY_CGAMEMOVEMENT_HPP
