//
// Created by sh1rm4n on 5/17/26.
//

#include "BunnyHop.hpp"

#include "../core/globals.hpp"
#include "../core/utils.hpp"

#include "../sourcesdk/typedefs.hpp"
#include "../sourcesdk/UserCmd.hpp"

#include "../sourcesdk/entities/C_BaseEntity.hpp"

void BunnyHop::Autostrafe(UserCmd *cmd, AutoStrafeType type) noexcept {
    if (!cmd || !type)
        return;

    if (!globals::core::localPlayer)
        return;

    if (globals::core::localPlayer->GetFlags() & FL_INWATER)
        return;

    if (globals::core::localPlayer->GetFlags() & FL_SWIM)
        return;

    if (globals::core::localPlayer->GetMoveType() == MOVETYPE_NOCLIP)
        return;

    if (globals::core::localPlayer->GetMoveType() == MOVETYPE_FLY)
        return;

    if (globals::core::localPlayer->GetMoveType() == MOVETYPE_LADDER)
        return;

    if (cmd->buttons & IN_ATTACK)
        return;

    const auto forwardSpeed = interfaces->cvar->FindCommandBase("cl_forwardspeed");

    if (type == Directional) {
        const auto velocity = globals::core::localPlayer->GetVelocity();
        const float speed2D = velocity.Length2D();

        // Invalid or too slow
        if (speed2D < 0.5f || !std::isfinite(speed2D)) {
            cmd->forwardMove = 10000.f;
            return;
        }

        const auto oldAngles = cmd->viewAngles;
        const auto oldSideMove = cmd->sideMove;
        const auto oldForwardMove = cmd->forwardMove;

        float yaw_modifier = 0.0f;

        if (cmd->buttons & IN_BACK) {
            yaw_modifier = 180.0f;

            if (cmd->buttons & IN_MOVELEFT) {
                yaw_modifier -= 45.0f;
            }
            else if (cmd->buttons & IN_MOVERIGHT) {
                yaw_modifier += 45.0f;
            }
        }
        else {
            if (cmd->buttons & IN_MOVELEFT) {
                yaw_modifier = (cmd->buttons & IN_FORWARD) ? 45.0f : 90.0f;
            }
            else if (cmd->buttons & IN_MOVERIGHT) {
                yaw_modifier = (cmd->buttons & IN_FORWARD) ? -45.0f : -90.0f;
            }
        }

        cmd->viewAngles.y += yaw_modifier;

        // Mouse-based side bias
        if (std::fabs(static_cast<float>(cmd->mouseX)) > 2.f)
            cmd->sideMove = (cmd->mouseY < 0) ? -10000.f : 10000.f;

        // Calculate velocity angle once
        const float velYaw = math::Rad2Deg(std::atan2(velocity.y, velocity.x));
        const float delta = math::NormalizeYaw(cmd->viewAngles.y - velYaw);

        // Directional strafe
        cmd->sideMove = (delta > 0.f) ? -10000.f : 10000.f;

        // Align view with velocity
        cmd->viewAngles.y = math::NormalizeYaw(cmd->viewAngles.y - delta);

        //utils::CorrectMovement(oldAngles, cmd, oldForwardMove, oldSideMove);

        // Forward boost scaling (keeps large values)
        cmd->forwardMove = std::clamp(5850.f / speed2D, -10000.f, 10000.f);
    }
}

void BunnyHop::Run(UserCmd *cmd) noexcept {
    if (!globals::core::localPlayer)
        return;

    if (globals::core::localPlayer->GetFlags() & FL_INWATER)
        return;

    if (globals::core::localPlayer->GetFlags() & FL_SWIM)
        return;

    if (globals::core::localPlayer->GetMoveType() == MOVETYPE_NOCLIP)
        return;

    if (globals::core::localPlayer->GetMoveType() == MOVETYPE_FLY)
        return;

    if (globals::core::localPlayer->GetMoveType() == MOVETYPE_LADDER)
        return;

    if (!(globals::core::localPlayer->GetFlags() & FL_ONGROUND)) {
        Autostrafe(cmd, Directional);
        cmd->buttons &= ~IN_JUMP;
    }
}