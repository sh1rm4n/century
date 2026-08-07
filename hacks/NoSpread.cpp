//
// Created by sh1rm4n on 7/30/26.
//

#include "NoSpread.hpp"

#include "../core/globals.hpp"
#include "../core/luaexecution.hpp"
#include "../core/utils.hpp"

#include "../sourcesdk/CUserCmd.hpp"
#include "../sourcesdk/entities/C_BaseCombatWeapon.hpp"
#include "../sourcesdk/entities/C_BaseEntity.hpp"
#include "../sourcesdk/interfaces/CLuaInterface.hpp"

std::string GetLuaBase(C_BaseCombatWeapon* weapon) noexcept {
    auto base = std::string("none");

    if (!weapon || !weapon->UsesLua())
        return base;

    const auto state = luaExecution->GetLuaState(0);

    weapon->PushEntity();

    lua_getfield(state, -1, "Base");

    base = lua_tostring(state, -1);

    lua_pop(state, 2);

    return base;
}

void LuaRandomSeed(lua_State* state, double seed) noexcept {
    if (!seed)
        return;

    const auto top = lua_gettop(state);

    lua_getfield(state, LUA_GLOBALSINDEX, "math");
    lua_getfield(state, -1, "randomseed");
    lua_pushnumber(state, seed);
    lua_call(state, 1, 0);

    lua_settop(state, top);
}

double LuaRandomFloat(lua_State* state, const double min, const double max) noexcept {
    if (!state || min == 0.0f || max == 0.0f)
        return 0.0f;

    const auto top = lua_gettop(state);

    lua_getfield(state, LUA_GLOBALSINDEX, "math");
    lua_getfield(state, -1, "Rand");
    lua_pushnumber(state, min);
    lua_pushnumber(state, max);
    lua_call(state, 2, 1);

    const auto result = lua_tonumber(state, -1);

    lua_settop(state, top);

    return result;
}

void NoSpread::SetSpread(const Vector &spread) noexcept {
    gunSpread = spread;
}

Angle NoSpread::LuaBasedSpread(const double cone, const double seed) noexcept {
    if (!seed)
        return {0,0,0};

    const auto state = luaExecution->GetLuaState(0);

    LuaRandomSeed(state, seed);

    const auto spread = Angle{
        static_cast<float>(LuaRandomFloat(state, -cone, cone)),
        static_cast<float>(LuaRandomFloat(state, -cone, cone)),
        0};

    return spread;
}

Vector NoSpread::EngineBasedSpread(const Vector &spread, float bias, CUserCmd* cmd) noexcept {
    float x, y, z;

    if ( bias > 1.0 )
        bias = 1.0;
    else if ( bias < 0.0 )
        bias = 0.0;

    const auto shotBiasMin = interfaces->cvar->FindCommandBase("ai_shot_bias_min")->GetFloat();
    const auto shotBiasMax = interfaces->cvar->FindCommandBase("ai_shot_bias_max")->GetFloat();

    const auto shotBias = ( ( shotBiasMax - shotBiasMin ) * bias ) + shotBiasMin;

    const auto flatness = std::fabs(shotBias) * 0.5;

    static auto randomSeed = reinterpret_cast<void(*)(int)>(
        modules->GetExport(modules->vstdlib, "RandomSeed"));

    static auto randomFloat = reinterpret_cast<float(*)(float, float)>(
        modules->GetExport(modules->vstdlib, "RandomFloat"));

    const int seed = MD5_PseudoRandom(cmd->commandNumber) & 0x7FFFFFFF;
    randomSeed(seed & 0xFF);

    do
    {
        x = randomFloat(-1,1) * flatness + randomFloat(-1,1) * (1 - flatness);
        y = randomFloat(-1,1) * flatness + randomFloat(-1,1) * (1 - flatness);
        if ( shotBias < 0 )
        {
            x = ( x >= 0 ) ? 1.0 - x : -1.0 - x;
            y = ( y >= 0 ) ? 1.0 - y : -1.0 - y;
        }
        z = x*x+y*y;
    } while (z > 1);

    cmd->viewAngles.Vectors(shootDirection, right, up);

    const auto result = shootDirection + (right * (x * spread.x)) + (up * (y * spread.y));

    return result;
}

void NoSpread::Run(CUserCmd* cmd) noexcept {
    if (!cmd || !globals::Core::localPlayer)
        return;

    if (!(cmd->buttons & IN_ATTACK))
        return;

    const auto weapon = globals::Core::localPlayer->GetActiveWeapon();
    if (!weapon)
        return;

    const auto oldAngles = cmd->viewAngles;
    const auto oldForwardMove = cmd->forwardMove;
    const auto oldSideMove = cmd->sideMove;

    if (weapon->UsesLua()) {
        const auto state = luaExecution->GetLuaState(0);
        if (!state)
            return;

        const auto base = GetLuaBase(weapon);

        if (base == "swb_base") {
            const int top = lua_gettop(state);

            weapon->PushEntity();

            lua_getfield(state, -1, "CurCone");

            auto cone = lua_tonumber(state, -1);

            lua_settop(state, top);

            if (cmd->buttons & IN_DUCK)
                cone *= 0.85f;

            cmd->viewAngles -= LuaBasedSpread(cone, cmd->commandNumber) * 25;
            cmd->viewAngles -= globals::Core::localPlayer->GetPunchAngles();
            return;
        }

        if (base == "cw_base") {
            const int top = lua_gettop(state);

            weapon->PushEntity();

            lua_getfield(state, -1, "CurCone");

            auto cone = lua_tonumber(state, -1);

            lua_settop(state, top);

            if (cmd->buttons & IN_DUCK)
                cone *= 0.85f;

            cmd->viewAngles -= LuaBasedSpread(cone, cmd->commandNumber) * 25;
            cmd->viewAngles -= globals::Core::localPlayer->GetPunchAngles();
            return;
        }

        if (base == "fas2_base") {
            const int top = lua_gettop(state);

            weapon->PushEntity();

            lua_getfield(state, -1, "CurCone");

            auto cone = lua_tonumber(state, -1);

            lua_settop(state, top);

            //if (cmd->buttons & IN_DUCK)
            //    cone *= 0.85f;

            cmd->viewAngles -= LuaBasedSpread(cone, interfaces->globalVars->curTime) * 24.9;
            cmd->viewAngles -= globals::Core::localPlayer->GetPunchAngles();
            return;
        }

        if (base == "weapon_cs_base2") {
            cmd->viewAngles += cmd->viewAngles - Angle::FromVector(EngineBasedSpread(gunSpread, 1.0f, cmd));
            cmd->viewAngles -= globals::Core::localPlayer->GetPunchAngles();
            return;
        }

        // tfa nospread is almost impossible, because of how to reimplement the cone calculation func..
        // and for arccw im too lazy, tho it maybe useless cuz of desync bullets feature

        if (base != "none")
            cmd->viewAngles += cmd->viewAngles - Angle::FromVector(EngineBasedSpread(gunSpread, 1.0f, cmd));
    }
    else {
        cmd->viewAngles += cmd->viewAngles - Angle::FromVector(EngineBasedSpread(gunSpread, 1.0f, cmd));
    }

    //utils::CorrectMovement(oldAngles, cmd, oldForwardMove, oldSideMove);
}