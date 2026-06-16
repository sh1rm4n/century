//
// Created by sh1rm4n on 6/16/26.
//

#ifndef CENTURY_CPREDICTION_HPP
#define CENTURY_CPREDICTION_HPP

class UserCmd;
class C_BaseEntity;
class CMoveHelperClient;
class CMoveData;

class CGMODPrediction {
public:
    constexpr void SetupMove(C_BaseEntity* entity, UserCmd* cmd, CMoveHelperClient* helper, CMoveData* moveData) noexcept {
        memory::Call<void>(this, 19, entity, cmd, helper, moveData);
    }

    constexpr void FinishMove(C_BaseEntity* entity, UserCmd* cmd, CMoveData* moveData) noexcept {
        memory::Call<void>(this, 20, entity, cmd, moveData);
    }
};

#endif //CENTURY_CPREDICTION_HPP
