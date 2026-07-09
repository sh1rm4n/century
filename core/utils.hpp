//
// Created by sh1rm4n on 5/16/26.
//

#ifndef CENTURY_UTILS_HPP
#define CENTURY_UTILS_HPP

#pragma once

#include "memory.hpp"
#include "modules.hpp"
#include "interfaces.hpp"
#include "netvars.hpp"

#include <imgui.h>

#include "../3rd/checksum_md5.h"

#include "../sourcesdk/math.hpp"
#include "../sourcesdk/UserCmd.hpp"
#include "../sourcesdk/typedefs.hpp"

#include "../sourcesdk/interfaces/CConVar.hpp"

#define CONCOLORMSG_EXPORT "_Z11ConColorMsgRK5ColorPKcz"

namespace utils {
    template<typename... Args>
    void Print(const std::format_string<Args...> fmt, Args &&... args) noexcept {
        const static auto conColorMsgAddr = modules->GetExport(modules->tier0, CONCOLORMSG_EXPORT);
        if (!conColorMsgAddr)
            return;

        using _ConColorMsg = void(*)(const Color &, const char *, ...);
        const static auto ConColorMsg = reinterpret_cast<_ConColorMsg>(conColorMsgAddr);
        if (!ConColorMsg)
            return;

        Color col = Color(0, 140, 255, 128);

        const auto formatted_str = std::format(fmt, std::forward<Args>(args)...);

        ConColorMsg(col, "%s", (formatted_str + "\n").c_str());
    }

    template<typename... Args>
    void Warn(const std::format_string<Args...> fmt, Args &&... args) noexcept {
        const static auto conColorMsgAddr = modules->GetExport(modules->tier0, CONCOLORMSG_EXPORT);
        if (!conColorMsgAddr)
            return;

        using _ConColorMsg = void(*)(const Color &, const char *, ...);
        const static auto ConColorMsg = reinterpret_cast<_ConColorMsg>(conColorMsgAddr);
        if (!ConColorMsg)
            return;

        Color col = Color(255, 184, 0, 128);

        const auto formatted_str = std::format(fmt, std::forward<Args>(args)...);

        ConColorMsg(col, "%s", (formatted_str + "\n").c_str());
    }

    template<typename... Args>
    void Error(const std::format_string<Args...> fmt, Args &&... args) noexcept {
        const static auto conColorMsgAddr = modules->GetExport(modules->tier0, CONCOLORMSG_EXPORT);
        if (!conColorMsgAddr)
            return;

        using _ConColorMsg = void(*)(const Color &, const char *, ...);
        const static auto ConColorMsg = reinterpret_cast<_ConColorMsg>(conColorMsgAddr);
        if (!ConColorMsg)
            return;

        Color col = Color(255, 0, 0, 128);

        const auto formatted_str = std::format(fmt, std::forward<Args>(args)...);

        ConColorMsg(col, "%s", (formatted_str + "\n").c_str());
    }

    template<typename... Args>
    void ColorPrint(const Color &col, std::format_string<Args...> fmt, Args &&... args) noexcept {
        static auto conColorMsgAddr = modules->GetExport(modules->tier0, CONCOLORMSG_EXPORT);
        if (!conColorMsgAddr)
            return;

        using _ConColorMsg = void(*)(const Color &, const char *, ...);
        const auto ConColorMsg = reinterpret_cast<_ConColorMsg>(conColorMsgAddr);

        std::string formatted_str = std::format(fmt, std::forward<Args>(args)...);

        ConColorMsg(col, "%s", (formatted_str + "\n").c_str());
    }

    inline C_BaseEntity *GetLocalPlayer() noexcept {
        using _CBasePlayer_GetLocalPlayer = C_BaseEntity*(*)();
        static auto CBasePlayer_GetLocalPlayer = reinterpret_cast<_CBasePlayer_GetLocalPlayer>(memory::FindPattern(
            "client_client.so", "55 48 8B 05 ? ? ? ? 48 89 E5 5D C3 90 66 90 48 8B 3D"));

        return CBasePlayer_GetLocalPlayer();
    }

    inline void PushAllowBoneAccess(bool allowForNormalModels, bool allowForViewModels, const std::string &tagPush) {
        using _CBaseAnimating_PushAllowBoneAccess = void(*)(bool, bool, const char *);
        static auto CBaseAnimating_PushAllowBoneAccess = reinterpret_cast<_CBaseAnimating_PushAllowBoneAccess>(
            memory::FindPattern("client_client.so", "55 48 89 E5 41 56 41 55 4C 8D 35 ? ? ? ? 49 89 D5"));

        CBaseAnimating_PushAllowBoneAccess(allowForNormalModels, allowForViewModels, tagPush.c_str());
    }

    inline void PopBoneAccess(const std::string &tagPush) {
        using _CBaseAnimating_PopBoneAccess = void(*)(const char *);
        static auto CBaseAnimating_PopBoneAccess = reinterpret_cast<_CBaseAnimating_PopBoneAccess>(memory::FindPattern(
            "client_client.so", "55 48 89 E5 53 48 8D 1D ? ? ? ? 48 83 EC ? E8"));

        CBaseAnimating_PopBoneAccess(tagPush.c_str());
    }

    static bool WorldToScreen(const Vector& world, Vector& screen) {
        // 1. Grab the active view-projection matrix from the Source Engine interface
        // This matrix transforms 3D world coordinates into 2D clip space coordinates
        const Matrix3x4& viewMatrix = interfaces->engineClient->WorldToScreenMatrix();

        // 2. Calculate the W component (Depth/Clip distance)
        float w = viewMatrix[3][0] * world.x +
                  viewMatrix[3][1] * world.y +
                  viewMatrix[3][2] * world.z +
                  viewMatrix[3][3];

        // Crucial Guard: If W is negative or near zero, the object is behind our camera perspective.
        // Returning false prevents the ESP text from mirroring onto the screen layout backwards.
        if (w < 0.01f)
            return false;

        // 3. Calculate X and Y positions via the transformation matrix
        float x = viewMatrix[0][0] * world.x +
                  viewMatrix[0][1] * world.y +
                  viewMatrix[0][2] * world.z +
                  viewMatrix[0][3];

        float y = viewMatrix[1][0] * world.x +
                  viewMatrix[1][1] * world.y +
                  viewMatrix[1][2] * world.z +
                  viewMatrix[1][3];

        // 4. Perspective Division (Transforms clip space down to Normalized Device Coordinates)
        x /= w;
        y /= w;

        // 5. Pull the current frame canvas size directly from ImGui
        // This ensures the screen scale perfectly maps to your overlay setup
        float screenWidth  = ImGui::GetIO().DisplaySize.x;
        float screenHeight = ImGui::GetIO().DisplaySize.y;

        // 6. Map NDC (-1.0 to 1.0) directly onto monitor pixel bounds
        // Notice the subtraction for Y: 2D Screen space runs downwards, 3D space runs upwards.
        screen.x = (screenWidth  / 2.f) + (x * screenWidth  / 2.f);
        screen.y = (screenHeight / 2.f) - (y * screenHeight / 2.f);

        return true;
    }

    static void CorrectMovement(const Angle oldViewAngles, UserCmd* cmd, float oldForwardMove, float oldSideMove)
    {
        Vector forward, right, up;
        oldViewAngles.Vectors(forward, right, up);

        forward.z = 0.0f;
        right.z = 0.0f;
        forward.Normalize();
        right.Normalize();

        Vector wishVel = (forward * oldForwardMove) + (right * oldSideMove);

        Vector newForward, newRight, newUp;
        cmd->viewAngles.Vectors(newForward, newRight, newUp);

        newForward.z = 0.0f;
        newRight.z = 0.0f;
        newForward.Normalize();
        newRight.Normalize();

        float totalDivide = (newForward.x * newRight.y - newForward.y * newRight.x);
        if (totalDivide != 0.0f) {
            cmd->forwardMove = (wishVel.x * newRight.y - wishVel.y * newRight.x) / totalDivide;
            cmd->sideMove    = (wishVel.y * newForward.x - wishVel.x * newForward.y) / totalDivide;
        }
    }

    static void Draw3DBox(ImDrawList *drawList, const Vector origin, const Angle angles, const Vector mins,
                          const Vector maxs, ImColor color = {255, 255, 255, 255}, float thickness = 1.5f) noexcept {
        Vector forward, right, up;
        angles.Vectors(forward, right, up);

        const std::array<Vector, 8> localCorners = {
            {
                {mins.x, mins.y, mins.z}, // 0: Bottom-back-left
                {mins.x, maxs.y, mins.z}, // 1: Bottom-front-left
                {maxs.x, maxs.y, mins.z}, // 2: Bottom-front-right
                {maxs.x, mins.y, mins.z}, // 3: Bottom-back-right
                {mins.x, mins.y, maxs.z}, // 4: Top-back-left
                {mins.x, maxs.y, maxs.z}, // 5: Top-front-left
                {maxs.x, maxs.y, maxs.z}, // 6: Top-front-right
                {maxs.x, mins.y, maxs.z} // 7: Top-back-right
            }
        };

        std::array<ImVec2, 8> screenCorners{};

        for (size_t i = 0; i < 8; ++i) {
            Vector worldPos = origin +
                              (forward * localCorners[i].x) +
                              (right * localCorners[i].y) +
                              (up * localCorners[i].z);

            Vector screenPos;
            if (!utils::WorldToScreen(worldPos, screenPos)) {
                return;
            }

            screenCorners[i] = ImVec2(screenPos.x, screenPos.y);
        }

        drawList->AddLine(screenCorners[0], screenCorners[1], color, thickness);
        drawList->AddLine(screenCorners[1], screenCorners[2], color, thickness);
        drawList->AddLine(screenCorners[2], screenCorners[3], color, thickness);
        drawList->AddLine(screenCorners[3], screenCorners[0], color, thickness);

        drawList->AddLine(screenCorners[4], screenCorners[5], color, thickness);
        drawList->AddLine(screenCorners[5], screenCorners[6], color, thickness);
        drawList->AddLine(screenCorners[6], screenCorners[7], color, thickness);
        drawList->AddLine(screenCorners[7], screenCorners[4], color, thickness);

        drawList->AddLine(screenCorners[0], screenCorners[4], color, thickness);
        drawList->AddLine(screenCorners[1], screenCorners[5], color, thickness);
        drawList->AddLine(screenCorners[2], screenCorners[6], color, thickness);
        drawList->AddLine(screenCorners[3], screenCorners[7], color, thickness);
    }

    static void DrawCornered3DBox(ImDrawList* drawList, const Vector origin, const Angle angles,
                      const Vector mins, const Vector maxs,
                      ImColor color = {255, 255, 255, 255}, float thickness = 1.5f) noexcept {
        Vector forward, right, up;
        angles.Vectors(forward, right, up);

        const std::array<Vector, 8> localCorners = {{
            {mins.x, mins.y, mins.z}, // 0: Bottom-back-left
            {mins.x, maxs.y, mins.z}, // 1: Bottom-front-left
            {maxs.x, maxs.y, mins.z}, // 2: Bottom-front-right
            {maxs.x, mins.y, mins.z}, // 3: Bottom-back-right
            {mins.x, mins.y, maxs.z}, // 4: Top-back-left
            {mins.x, maxs.y, maxs.z}, // 5: Top-front-left
            {maxs.x, maxs.y, maxs.z}, // 6: Top-front-right
            {maxs.x, mins.y, maxs.z}  // 7: Top-back-right
        }};

        std::array<ImVec2, 8> s{};
        for (size_t i = 0; i < 8; ++i) {
            Vector worldPos = origin
                + forward * localCorners[i].x
                + right   * localCorners[i].y
                + up      * localCorners[i].z;
            Vector screenPos;
            if (!utils::WorldToScreen(worldPos, screenPos))
                return;
            s[i] = { screenPos.x, screenPos.y };
        }

        // what fraction of each edge to draw at each end
        constexpr float f = 0.25f;

        // helper — draws corners at both ends of an edge
        const auto corner = [&](ImVec2 a, ImVec2 b) {
            const float dx = (b.x - a.x) * f;
            const float dy = (b.y - a.y) * f;
            drawList->AddLine(a,               { a.x + dx, a.y + dy }, color, thickness);
            drawList->AddLine(b,               { b.x - dx, b.y - dy }, color, thickness);
        };

        // bottom face
        corner(s[0], s[1]);
        corner(s[1], s[2]);
        corner(s[2], s[3]);
        corner(s[3], s[0]);
        // top face
        corner(s[4], s[5]);
        corner(s[5], s[6]);
        corner(s[6], s[7]);
        corner(s[7], s[4]);
        // verticals
        corner(s[0], s[4]);
        corner(s[1], s[5]);
        corner(s[2], s[6]);
        corner(s[3], s[7]);
    }

    static void DrawBones(ImDrawList* drawList, const Matrix3x4* bones) noexcept {
        // pelvis -> spines -> head
        Vector bone0 = bones[0].GetOrigin();
        Vector bone1 = bones[1].GetOrigin();
        Vector bone2 = bones[2].GetOrigin();
        Vector bone3 = bones[3].GetOrigin();
        Vector bone4 = bones[4].GetOrigin();
        Vector bone5 = bones[5].GetOrigin();
        Vector bone6 = bones[6].GetOrigin();
        Vector bone7 = bones[7].GetOrigin();
        // shoulders -> arms -> hands

        // left arm
        Vector bone8 = bones[8].GetOrigin();
        Vector bone9 = bones[9].GetOrigin();
        Vector bone10 = bones[10].GetOrigin();
        Vector bone11 = bones[11].GetOrigin();
        Vector bone12 = bones[12].GetOrigin();

        // right arm
        Vector bone13 = bones[13].GetOrigin();
        Vector bone14 = bones[14].GetOrigin();
        Vector bone15 = bones[15].GetOrigin();
        Vector bone16 = bones[16].GetOrigin();
        Vector bone17 = bones[17].GetOrigin();

        // pelvis -> legs

        // left leg
        Vector bone18 = bones[18].GetOrigin();
        Vector bone19 = bones[19].GetOrigin();
        Vector bone20 = bones[20].GetOrigin();
        Vector bone21 = bones[21].GetOrigin();
        // right leg
        Vector bone22 = bones[22].GetOrigin();
        Vector bone23 = bones[23].GetOrigin();
        Vector bone24 = bones[24].GetOrigin();
        Vector bone25 = bones[25].GetOrigin();

        Vector outBone0;
        Vector outBone1;
        Vector outBone2;
        Vector outBone3;
        Vector outBone4;
        Vector outBone5;
        Vector outBone6;
        Vector outBone7;
        Vector outBone8;
        Vector outBone9;
        Vector outBone10;
        Vector outBone11;
        Vector outBone12;
        Vector outBone13;
        Vector outBone14;
        Vector outBone15;
        Vector outBone16;
        Vector outBone17;
        Vector outBone18;
        Vector outBone19;
        Vector outBone20;
        Vector outBone21;
        Vector outBone22;
        Vector outBone23;
        Vector outBone24;
        Vector outBone25;

        if (utils::WorldToScreen(bone0, outBone0) &&
        utils::WorldToScreen(bone1, outBone1) &&
        utils::WorldToScreen(bone2, outBone2) &&
        utils::WorldToScreen(bone3, outBone3) &&
        utils::WorldToScreen(bone4, outBone4) &&
        utils::WorldToScreen(bone5, outBone5) &&
        utils::WorldToScreen(bone6, outBone6) &&
        utils::WorldToScreen(bone7, outBone7) &&
        utils::WorldToScreen(bone8, outBone8) &&
        utils::WorldToScreen(bone9, outBone9) &&
        utils::WorldToScreen(bone10, outBone10) &&
        utils::WorldToScreen(bone11, outBone11) &&
        utils::WorldToScreen(bone12, outBone12) &&
        utils::WorldToScreen(bone13, outBone13) &&
        utils::WorldToScreen(bone14, outBone14) &&
        utils::WorldToScreen(bone15, outBone15) &&
        utils::WorldToScreen(bone16, outBone16) &&
        utils::WorldToScreen(bone17, outBone17) &&
        utils::WorldToScreen(bone18, outBone18) &&
        utils::WorldToScreen(bone19, outBone19) &&
        utils::WorldToScreen(bone20, outBone20) &&
        utils::WorldToScreen(bone21, outBone21) &&
        utils::WorldToScreen(bone22, outBone22) &&
        utils::WorldToScreen(bone23, outBone23) &&
        utils::WorldToScreen(bone24, outBone24) &&
        utils::WorldToScreen(bone25, outBone25))
        {
            drawList->AddLine({outBone0.x, outBone0.y}, {outBone1.x, outBone1.y}, ImColor{255,255,255});
            drawList->AddLine({outBone1.x, outBone1.y}, {outBone2.x, outBone2.y}, ImColor{255,255,255});
            drawList->AddLine({outBone2.x, outBone2.y}, {outBone3.x, outBone3.y}, ImColor{255,255,255});
            drawList->AddLine({outBone3.x, outBone3.y}, {outBone4.x, outBone4.y}, ImColor{255,255,255});
            drawList->AddLine({outBone4.x, outBone4.y}, {outBone5.x, outBone5.y}, ImColor{255,255,255});
            drawList->AddLine({outBone5.x, outBone5.y}, {outBone6.x, outBone6.y}, ImColor{255,255,255});
            drawList->AddLine({outBone6.x, outBone6.y}, {outBone7.x, outBone7.y}, ImColor{255,255,255});
            drawList->AddLine({outBone4.x, outBone4.y}, {outBone8.x, outBone8.y}, ImColor{255,255,255});
            drawList->AddLine({outBone8.x, outBone8.y}, {outBone9.x, outBone9.y}, ImColor{255,255,255});
            drawList->AddLine({outBone9.x, outBone9.y}, {outBone10.x, outBone10.y}, ImColor{255,255,255});
            drawList->AddLine({outBone10.x, outBone10.y}, {outBone11.x, outBone11.y}, ImColor{255,255,255});
            drawList->AddLine({outBone11.x, outBone11.y}, {outBone12.x, outBone12.y}, ImColor{255,255,255});
            drawList->AddLine({outBone4.x, outBone4.y}, {outBone13.x, outBone13.y}, ImColor{255,255,255});
            drawList->AddLine({outBone13.x, outBone13.y}, {outBone14.x, outBone14.y}, ImColor{255,255,255});
            drawList->AddLine({outBone14.x, outBone14.y}, {outBone15.x, outBone15.y}, ImColor{255,255,255});
            drawList->AddLine({outBone15.x, outBone15.y}, {outBone16.x, outBone16.y}, ImColor{255,255,255});
            drawList->AddLine({outBone16.x, outBone16.y}, {outBone17.x, outBone17.y}, ImColor{255,255,255});
            drawList->AddLine({outBone0.x, outBone0.y}, {outBone18.x, outBone18.y}, ImColor{255,255,255});
            drawList->AddLine({outBone18.x, outBone18.y}, {outBone19.x, outBone19.y}, ImColor{255,255,255});
            drawList->AddLine({outBone19.x, outBone19.y}, {outBone20.x, outBone20.y}, ImColor{255,255,255});
            drawList->AddLine({outBone20.x, outBone20.y}, {outBone21.x, outBone21.y}, ImColor{255,255,255});
            drawList->AddLine({outBone0.x, outBone0.y}, {outBone22.x, outBone22.y}, ImColor{255,255,255});
            drawList->AddLine({outBone22.x, outBone22.y}, {outBone23.x, outBone23.y}, ImColor{255,255,255});
            drawList->AddLine({outBone23.x, outBone23.y}, {outBone24.x, outBone24.y}, ImColor{255,255,255});
            drawList->AddLine({outBone24.x, outBone24.y}, {outBone25.x, outBone25.y}, ImColor{255,255,255});
        }
    }

    static bool VisCheck(C_BaseEntity* filterEntity, C_BaseEntity* targetEntity, const Vector& start, const Vector& end) noexcept {
        CTraceFilterTest filter(filterEntity);
        Ray ray;
        ray.Init(start, end);
        Trace trace;

        interfaces->engineTrace->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &trace);

        if (trace.startSolid || trace.allSolid)
            return false;

        const bool hitTarget = static_cast<C_BaseEntity*>(trace.entity) == targetEntity;
        const bool notBlocked = trace.fraction >= 0.97f;

        return (hitTarget || notBlocked);
    }

    static void SetPredictionRandomSeed(const std::uint32_t seed) noexcept {
        static auto predictionRandomSeed = memory::ResolveGOT(memory::GetAbsAddress(memory::FindPattern("client_client.so", "48 8B 05 ? ? ? ? 55 48 89 E5 41 54 41 89 D4")));

        *reinterpret_cast<std::uint32_t*>(predictionRandomSeed) = seed;
    }

    static void SetPredictionPlayer(const std::uint32_t entityHandle) noexcept {
        static auto predictionPlayer = memory::ResolveGOT(memory::GetAbsAddress(memory::FindPattern("client_client.so", "48 8B 05 ? ? ? ? ? ? 75 ? 48 83 C4")));

        *reinterpret_cast<std::uint32_t*>(predictionPlayer) = entityHandle;
    }

    static void ResetPredictionInstanceCounters() {
        using _CPredictableID_ResetPredictionInstanceCounters = void(*)();
        static auto CPredictableID_ResetPredictionInstanceCounters = reinterpret_cast<_CPredictableID_ResetPredictionInstanceCounters>(memory::FindPattern(
            "client_client.so", "55 BA ? ? ? ? 31 F6 48 8D 3D ? ? ? ? 48 89 E5 5D"));

        CPredictableID_ResetPredictionInstanceCounters();
    }

    static std::uint32_t PseudoRandom(const std::uint32_t& seed) noexcept {
        return MD5_PseudoRandom(seed);
    }

    static void SetSendPacket(const bool value) noexcept {
        static auto bSendPacketAddr = memory::FindPattern("engine_client.so", "BB ? ? ? ? E9 ? ? ? ? 66 0F 1F 44 00 ? 48 8B 05 ? ? ? ? F2 0F 10 05") + 0x1;

        static auto* bSendPacket = reinterpret_cast<std::uint32_t*>(bSendPacketAddr);

        static std::uint64_t pageSize = sysconf(_SC_PAGESIZE);
        static auto page = reinterpret_cast<void*>(
            reinterpret_cast<std::uintptr_t>(bSendPacket) & ~(pageSize - 1)
        );

        if (mprotect(page, pageSize, PROT_READ | PROT_WRITE | PROT_EXEC) == 0) {
            *bSendPacket = static_cast<std::uint32_t>(value);

            mprotect(page, pageSize, PROT_READ | PROT_EXEC);
        }
    }

    static bool GetSendPacket() noexcept {
        static auto bSendPacketAddr = memory::FindPattern("engine_client.so", "BB ? ? ? ? E9 ? ? ? ? 66 0F 1F 44 00 ? 48 8B 05 ? ? ? ? F2 0F 10 05") + 0x1;

        static auto* bSendPacket = reinterpret_cast<std::uint32_t*>(bSendPacketAddr);

        return static_cast<bool>(*bSendPacket);
    }

    template <typename T>
    static T Random(T min, T max) noexcept {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_real_distribution<T> dist(min, max);
        return dist(gen);
    }

    static void UpdateClientSideAnimations() noexcept {
        using _CBaseAnimating_UpdateClientSideAnimations = void(*)();
        static auto CBaseAnimating_UpdateClientSideAnimations = reinterpret_cast<_CBaseAnimating_UpdateClientSideAnimations>(memory::FindPattern(
            "client_client.so", "55 48 89 E5 41 56 41 55 4C 8B 2D ? ? ? ? 41 54 53 41 8B 95 ? ? ? ? 85 D2 41 0F 95 C6 0F 85 ? ? ? ? 8B 05 ? ? ? ? 85 C0 7E ? 44 8D 60"));

        CBaseAnimating_UpdateClientSideAnimations();
    }

    static void InvalidateBoneCache() noexcept {
        using _CBaseAnimating_InvalidateBoneCache = void(*)();
        static auto CBaseAnimating_InvalidateBoneCache = reinterpret_cast<_CBaseAnimating_InvalidateBoneCache>(memory::FindPattern(
            "client_client.so", "48 83 05 ? ? ? ? ? 55"));

        CBaseAnimating_InvalidateBoneCache();
    }

    static void ChokePacket() noexcept {
        static auto chokedCommandsAddr = memory::GetAbsAddress(memory::FindPattern("engine_client.so", "83 05 ? ? ? ? ? 45 84 F6"));
        auto* chokedCommands = reinterpret_cast<std::uint32_t*>(chokedCommandsAddr);

        const auto netChan = interfaces->engineClient->GetNetChannelInfo();

        if (netChan) {
            netChan->SetChoked();
            *chokedCommands = *chokedCommands + 1;
        }
    }

    static float GetLerpTime() noexcept {
        const auto clInterp = 0.200000f;
        const auto clInterpRatio = 2.f;
        const auto clUpdateRate = 66;

        return std::max(clInterp, clInterpRatio / clUpdateRate);
    }

    inline int TimeToTicks(float time, float interval) noexcept {
        return static_cast<int>(0.5f + time / interval);
    }

    inline float TicksToTime(int ticks) noexcept {
        return ticks * interfaces->globalVars->interval_per_tick;
    }

    static std::uint32_t NetVar(const std::string& className, const std::string& varName) noexcept {
        return netVars->GetOffset(std::format("{}->{}", className, varName));
    }

    static int GetCorrectTickCount(float simTime, float lerpTime, float tickInterval) noexcept {
        const float lerpRemainder = std::fmod(lerpTime, tickInterval);
        float realTargetTime      = simTime;

        if (lerpRemainder > 0.f)
            realTargetTime += tickInterval - lerpRemainder;

        return TimeToTicks(realTargetTime + lerpTime, tickInterval);
    }
}

#endif //CENTURY_UTILS_HPP
