//
// Created by sh1rm4n on 5/16/26.
//

#ifndef CENTURY_UTILS_HPP
#define CENTURY_UTILS_HPP

#include <format>
#include <array>

#include "globals.hpp"
#include "memory.hpp"
#include "modules.hpp"
#include "interfaces.hpp"

#include <imgui.h>

#include "../sourcesdk/math.hpp"
#include "../sourcesdk/UserCmd.hpp"

#include "../sourcesdk/entities/C_BaseEntity.hpp"

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

    static void CorrectMovement(const Angle oldAngles, UserCmd* cmd, float oldForward, float oldSideMove)
    {
        Vector forward, right, up;
        oldAngles.Vectors(forward, right, up);

        // Zero out the Z axis to isolate direction on the horizontal plane
        forward.z = 0.0f;
        right.z = 0.0f;
        forward.Normalize();
        right.Normalize();

        // Reconstruct the user's intended velocity vector in world space
        Vector wishVel = (forward * oldForward) + (right * oldSideMove);

        Vector newForward, newRight, newUp;
        cmd->viewAngles.Vectors(newForward, newRight, newUp);

        newForward.z = 0.0f;
        newRight.z = 0.0f;
        newForward.Normalize();
        newRight.Normalize();

        // Solve the linear equations to perfectly project velocity onto the new vectors
        float totalDivide = (newForward.x * newRight.y - newForward.y * newRight.x);
        if (totalDivide != 0.0f) {
            cmd->forwardMove = (wishVel.x * newRight.y - wishVel.y * newRight.x) / totalDivide;
            cmd->sideMove    = (wishVel.y * newForward.x - wishVel.x * newForward.y) / totalDivide;
        }
    }

    static void Draw3DBox(ImDrawList *drawList, const Vector origin, const Angle angles, const Vector mins,
                          const Vector maxs) noexcept {
        // 1. Extract the local orientation axes from the angle
        Vector forward, right, up;
        angles.Vectors(forward, right, up);

        // 2. Define the 8 corners in local space (relative to 0,0,0)
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

        // 3. Transform local corners to world space, then project to 2D screen space
        std::array<ImVec2, 8> screenCorners{};

        for (size_t i = 0; i < 8; ++i) {
            // Rotate the point using the orientation matrix and add the world position origin
            Vector worldPos = origin +
                              (forward * localCorners[i].x) +
                              (right * localCorners[i].y) +
                              (up * localCorners[i].z);

            Vector screenPos;
            if (!utils::WorldToScreen(worldPos, screenPos)) {
                return; // Early exit if any corner is off-screen (cleaner than validPoints flags)
            }

            screenCorners[i] = ImVec2(screenPos.x, screenPos.y);
        }

        // 4. Render the oriented box lines
        constexpr ImColor boxColor{255, 255, 255, 255};
        constexpr float thickness = 1.0f;

        // Bottom Face
        drawList->AddLine(screenCorners[0], screenCorners[1], boxColor, thickness);
        drawList->AddLine(screenCorners[1], screenCorners[2], boxColor, thickness);
        drawList->AddLine(screenCorners[2], screenCorners[3], boxColor, thickness);
        drawList->AddLine(screenCorners[3], screenCorners[0], boxColor, thickness);

        // Top Face
        drawList->AddLine(screenCorners[4], screenCorners[5], boxColor, thickness);
        drawList->AddLine(screenCorners[5], screenCorners[6], boxColor, thickness);
        drawList->AddLine(screenCorners[6], screenCorners[7], boxColor, thickness);
        drawList->AddLine(screenCorners[7], screenCorners[4], boxColor, thickness);

        // Vertical Pillars
        drawList->AddLine(screenCorners[0], screenCorners[4], boxColor, thickness);
        drawList->AddLine(screenCorners[1], screenCorners[5], boxColor, thickness);
        drawList->AddLine(screenCorners[2], screenCorners[6], boxColor, thickness);
        drawList->AddLine(screenCorners[3], screenCorners[7], boxColor, thickness);
    }

    static bool VisCheck(const Vector& start, const Vector& end) noexcept {
        TraceFilterSimple filter(globals::core::localPlayer);

        Ray ray;
        ray.Init(start, end);

        Trace trace;
        interfaces->engineTrace->TraceRay(ray, 0, nullptr, nullptr);

        return trace.entity == globals::core::localPlayer || trace.fraction >= 0.98f;
    }

    static void SetPredictionSeed(const std::uint32_t& seed) noexcept {
        static auto predictionSeedAddr = memory::ResolveGOT("client_client.so", 0x12BEFB8);

        if (!predictionSeedAddr)
            return;

        *reinterpret_cast<std::uint32_t*>(predictionSeedAddr) = seed;
    }
}

#endif //CENTURY_UTILS_HPP
