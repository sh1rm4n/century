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
#include <fmt/format.h>

#include "../3rd/checksum_md5.h"

#include "../sourcesdk/math.hpp"
#include "../sourcesdk/CUserCmd.hpp"
#include "../sourcesdk/typedefs.hpp"

#include "../sourcesdk/interfaces/CConVar.hpp"

#if defined(__linux__)
#define CONCOLORMSG_EXPORT "_Z11ConColorMsgRK5ColorPKcz"
#else
#define CONCOLORMSG_EXPORT "?ConColorMsg@@YAXAEBVColor@@PEBDZZ"
#endif

namespace utils {
    template<typename... Args>
    void Print(const fmt::format_string<Args...> fmt, Args &&... args) noexcept {
        const static auto conColorMsgAddr = modules->GetExport(modules->tier0, CONCOLORMSG_EXPORT);
        if (!conColorMsgAddr)
            return;

        using _ConColorMsg = void(*)(const Color &, const char *, ...);
        const static auto ConColorMsg = reinterpret_cast<_ConColorMsg>(conColorMsgAddr);
        if (!ConColorMsg)
            return;

        Color col = Color(0, 140, 255, 128);

        const auto formatted_str = fmt::format(fmt, std::forward<Args>(args)...);

        ConColorMsg(col, "%s", (formatted_str + "\n").c_str());
    }

    template<typename... Args>
    void Warn(const fmt::format_string<Args...> fmt, Args &&... args) noexcept {
        const static auto conColorMsgAddr = modules->GetExport(modules->tier0, CONCOLORMSG_EXPORT);
        if (!conColorMsgAddr)
            return;

        using _ConColorMsg = void(*)(const Color &, const char *, ...);
        const static auto ConColorMsg = reinterpret_cast<_ConColorMsg>(conColorMsgAddr);
        if (!ConColorMsg)
            return;

        Color col = Color(255, 184, 0, 128);

        const auto formatted_str = fmt::format(fmt, std::forward<Args>(args)...);

        ConColorMsg(col, "%s", (formatted_str + "\n").c_str());
    }

    template<typename... Args>
    void Error(const fmt::format_string<Args...> fmt, Args &&... args) noexcept {
        const static auto conColorMsgAddr = modules->GetExport(modules->tier0, CONCOLORMSG_EXPORT);
        if (!conColorMsgAddr)
            return;

        using _ConColorMsg = void(*)(const Color &, const char *, ...);
        const static auto ConColorMsg = reinterpret_cast<_ConColorMsg>(conColorMsgAddr);
        if (!ConColorMsg)
            return;

        Color col = Color(255, 0, 0, 128);

        const auto formatted_str = fmt::format(fmt, std::forward<Args>(args)...);

        ConColorMsg(col, "%s", (formatted_str + "\n").c_str());
    }

    template<typename... Args>
    void ColorPrint(const Color &col, fmt::format_string<Args...> fmt, Args &&... args) noexcept {
        static auto conColorMsgAddr = modules->GetExport(modules->tier0, CONCOLORMSG_EXPORT);
        if (!conColorMsgAddr)
            return;

        using _ConColorMsg = void(*)(const Color &, const char *, ...);
        const auto ConColorMsg = reinterpret_cast<_ConColorMsg>(conColorMsgAddr);

        std::string formatted_str = fmt::format(fmt, std::forward<Args>(args)...);

        ConColorMsg(col, "%s", (formatted_str + "\n").c_str());
    }

    inline C_BaseEntity* GetLocalPlayer() noexcept {
        return interfaces->entityList->GetClientEntity<C_BaseEntity>(interfaces->engineClient->GetLocalPlayer());
    }

    static bool WorldToScreen(const Vector& world, Vector& screen) {
        const Matrix3x4& viewMatrix = interfaces->engineClient->WorldToScreenMatrix();

        float w = viewMatrix[3][0] * world.x +
                  viewMatrix[3][1] * world.y +
                  viewMatrix[3][2] * world.z +
                  viewMatrix[3][3];

        if (w < 0.01f)
            return false;

        float x = viewMatrix[0][0] * world.x +
                  viewMatrix[0][1] * world.y +
                  viewMatrix[0][2] * world.z +
                  viewMatrix[0][3];

        float y = viewMatrix[1][0] * world.x +
                  viewMatrix[1][1] * world.y +
                  viewMatrix[1][2] * world.z +
                  viewMatrix[1][3];

        x /= w;
        y /= w;

        float screenWidth  = ImGui::GetIO().DisplaySize.x;
        float screenHeight = ImGui::GetIO().DisplaySize.y;

        screen.x = (screenWidth  / 2.f) + (x * screenWidth  / 2.f);
        screen.y = (screenHeight / 2.f) - (y * screenHeight / 2.f);

        return true;
    }

    static void CorrectMovement(const Angle oldViewAngles, CUserCmd* cmd, float oldForwardMove, float oldSideMove)
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
                {mins.x, mins.y, mins.z},
                {mins.x, maxs.y, mins.z},
                {maxs.x, maxs.y, mins.z},
                {maxs.x, mins.y, mins.z},
                {mins.x, mins.y, maxs.z},
                {mins.x, maxs.y, maxs.z},
                {maxs.x, maxs.y, maxs.z},
                {maxs.x, mins.y, maxs.z}
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
            {mins.x, mins.y, mins.z},
            {mins.x, maxs.y, mins.z},
            {maxs.x, maxs.y, mins.z},
            {maxs.x, mins.y, mins.z},
            {mins.x, mins.y, maxs.z},
            {mins.x, maxs.y, maxs.z},
            {maxs.x, maxs.y, maxs.z},
            {maxs.x, mins.y, maxs.z}
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

    static std::uint32_t PseudoRandom(const std::uint32_t& seed) noexcept {
        return MD5_PseudoRandom(seed);
    }

    template <typename T>
    static T Random(T min, T max) noexcept {
        thread_local std::mt19937 gen{std::random_device{}()};

        if constexpr (std::is_integral_v<T>)
        {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(gen);
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(gen);
        }
        else
        {
            static_assert(std::is_arithmetic_v<T>, "Random<T> requires an arithmetic type.");
        }
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

    inline int TimeToTicks(float time, float interval) noexcept {
        return static_cast<int>(0.5f + time / interval);
    }

    inline float TicksToTime(int ticks) noexcept {
        return ticks * interfaces->globalVars->intervalPerTick;
    }

    static std::uint32_t NetVar(const std::string& className, const std::string& varName) noexcept {
        return netVars->GetOffset(fmt::format("{}->{}", className, varName));
    }
}

#endif //CENTURY_UTILS_HPP
