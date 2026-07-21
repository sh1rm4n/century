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

        memory::Call<void>(this, 18, player, ucmd, moveHelper);
    }

    constexpr void SetupMove(C_BaseEntity* entity, CUserCmd* cmd, CMoveHelperClient* helper, CMoveData* moveData) noexcept {
        memory::Call<void>(this, 19, entity, cmd, helper, moveData);
    }

    constexpr void FinishMove(C_BaseEntity* entity, CUserCmd* cmd, CMoveData* moveData) noexcept {
        memory::Call<void>(this, 20, entity, cmd, moveData);
    }

    constexpr void StartCommand(C_BaseEntity* player, CUserCmd* cmd) noexcept {
        using _CPrediction_StartCommand = void(*)(CGMODPrediction*, C_BaseEntity*, CUserCmd*);
        static auto CPrediction_StartCommand = reinterpret_cast<_CPrediction_StartCommand>(memory::FindPattern(
            "client_client.so", "55 48 89 E5 41 57 41 56 41 55 49 89 D5 41 54 49 89 F4 53 48 83 EC ? 48 8B 1D"));

        CPrediction_StartCommand(this, player, cmd);
    }

    constexpr void FinishCommand(C_BaseEntity* player) noexcept {
        using _CPrediction_FinishCommand = void(*)(CGMODPrediction*, C_BaseEntity*);
        static auto CPrediction_FinishCommand = reinterpret_cast<_CPrediction_FinishCommand>(memory::FindPattern(
            "client_client.so", "55 48 89 E5 41 56 41 55 49 89 F5 41 54 53 48 8B 1D"));

        CPrediction_FinishCommand(this, player);
    }

    constexpr void RunPreThink(C_BaseEntity* player) noexcept {
        using _CPrediction_RunPreThink = void(*)(CGMODPrediction*, C_BaseEntity*);
        static auto CPrediction_RunPreThink = reinterpret_cast<_CPrediction_RunPreThink>(memory::FindPattern(
            "client_client.so", "55 48 89 E5 41 56 41 55 41 54 49 89 F4 53 48 8B 1D ? ? ? ? 8B 8B ? ? ? ? 85 C9 41 0F 95 C5 0F 85 ? ? ? ? 31 F6 4C 89 E7 E8 ? ? ? ? 84 C0"));

        CPrediction_RunPreThink(this, player);
    }

    constexpr void RunThink(C_BaseEntity* player, double frameTime) noexcept {
        using _CPrediction_RunThink = void(*)(CGMODPrediction*, C_BaseEntity*, double);
        static auto CPrediction_RunThink = reinterpret_cast<_CPrediction_RunThink>(memory::FindPattern(
            "client_client.so", "55 48 89 E5 41 56 41 55 41 54 49 89 F4 53 48 8B 1D ? ? ? ? 8B 8B ? ? ? ? 85 C9 41 0F 95 C5 0F 85 ? ? ? ? 31 F6 4C 89 E7 E8 ? ? ? ? 85 C0"));

        CPrediction_RunThink(this, player, frameTime);
    }

    constexpr void RunPostThink(C_BaseEntity* player) noexcept {
        using _CPrediction_RunPostThink = void(*)(CGMODPrediction*, C_BaseEntity*);
        static auto CPrediction_RunPostThink = reinterpret_cast<_CPrediction_RunPostThink>(memory::FindPattern(
            "client_client.so", "55 48 89 E5 41 56 41 55 41 54 49 89 F4 53 48 8B 1D ? ? ? ? 8B 93"));

        CPrediction_RunPostThink(this, player);
    }

    constexpr void SetPredictionEnabled(const bool enabled) noexcept {
        *reinterpret_cast<bool*>(std::uint64_t(this) + 0xC) = enabled;
    }

    constexpr void SetFirstTimePredicted(const bool firstTimePredicted) noexcept {
        *reinterpret_cast<bool*>(std::uint64_t(this) + 0xD) = firstTimePredicted;
    }

    constexpr auto GetPredictionEnabled() noexcept {
        return *reinterpret_cast<bool*>(std::uint64_t(this) + 0xC);
    }

    constexpr auto GetFirstTimePredicted() noexcept {
        return *reinterpret_cast<bool*>(std::uint64_t(this) + 0xD);
    }
};

#endif //CENTURY_CPREDICTION_HPP
