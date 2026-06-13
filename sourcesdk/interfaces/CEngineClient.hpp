//
// Created by sh1rm4n on 5/18/26.
//

#ifndef CENTURY_CENGINECLIENT_HPP
#define CENTURY_CENGINECLIENT_HPP

#include "CNetChan.hpp"

#include "../../core/memory.hpp"

class ISpatialQuery;
class Matrix3x4;

typedef struct player_info_s
{
    char name[128];
    int userID; // local server user ID, unique while server is running <- THIS IS NOT STEAMID
    char guid[33]; // that is Steamid
    unsigned int friendsid;
    char friendsname[128];
    bool fakeplayer; // true, if player is a bot controlled by game.dll
    bool ishltv;
    unsigned int customfiles[4]; // custom files CRC for this player
    unsigned char filesdownloaded; // this counter increases each time the server downloaded a new file
} player_info_t;

class CEngineClient {
public:
    constexpr bool GetPlayerInfo(int index, player_info_s* info) noexcept {
        return memory::Call<bool>(this, 8, index, info);
    }

    constexpr int GetLocalPlayer() noexcept {
        return memory::Call<int>(this, 15);
    }

    constexpr void GetViewAngles(Angle& angles) noexcept {
        memory::Call<void>(this, 19, &angles);
    }

    constexpr void SetViewAngles(Angle& angles) noexcept {
        memory::Call<void>(this, 20, &angles);
    }

    constexpr bool IsInGame() noexcept {
        return memory::Call<bool>(this, 26);
    }

    constexpr Matrix3x4& WorldToScreenMatrix() noexcept {
        return memory::Call<Matrix3x4&>(this, 36);
    }

    constexpr ISpatialQuery* GetBSPTreeQuery() noexcept {
        return memory::Call<ISpatialQuery*>(this, 42);
    }

    constexpr CNetChan* GetNetChannelInfo() noexcept {
        return memory::Call<CNetChan*>(this, 72);
    }

    constexpr void ClientCmd_Unrestricted(const char* cmd) noexcept {
        memory::Call<void>(this, 106, cmd);
    }
};

#endif //CENTURY_CENGINECLIENT_HPP
