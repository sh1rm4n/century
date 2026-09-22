//
// Created by sh1rm4n on 6/16/26.
//

#ifndef CENTURY_CPREDICTION_HPP
#define CENTURY_CPREDICTION_HPP

class CUserCmd;
class C_BaseEntity;
class CMoveHelperClient;
class CMoveData;

class CGMODPrediction {
public:
    constexpr void RunCommand(C_BaseEntity *player, CUserCmd *ucmd, CMoveHelperClient *moveHelper) noexcept {
        if (!player|| !moveHelper)
            return;
#if defined(__linux__)
        memory::Call<void>(this, 18, player, ucmd, moveHelper);
#else
        memory::Call<void>(this, 17, player, ucmd, moveHelper);
#endif
    }
};

#endif //CENTURY_CPREDICTION_HPP
