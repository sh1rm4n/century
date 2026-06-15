//
// Created by sh1rm4n on 6/12/26.
//

#include "luaexecution.hpp"

#include <SDL_clipboard.h>

#include "interfaces.hpp"
#include "modules.hpp"
#include "utils.hpp"

#define lua_pop(L,n) lua_settop(L, -(n)-1)

void LuaExecution::Prepare() noexcept {
    lua_newthread = reinterpret_cast<lua_newthread_>(modules->GetExport(modules->luaShared, "lua_newthread"));
    luaL_loadbufferx = reinterpret_cast<luaL_loadbufferx_>(modules->GetExport(modules->luaShared, "luaL_loadbufferx"));
    lua_pcall = reinterpret_cast<lua_pcall_>(modules->GetExport(modules->luaShared, "lua_pcall"));
    lua_settop = reinterpret_cast<lua_settop_>(modules->GetExport(modules->luaShared, "lua_settop"));

    //utils::Warn("[century][lua execution] luaL_loadbufferx: {}", reinterpret_cast<std::uintptr_t>(luaL_loadbufferx));
    //utils::Warn("[century][lua execution] lua_pcall: {}", reinterpret_cast<std::uintptr_t>(lua_pcall));
    //utils::Warn("[century][lua execution] lua_settop: {}", reinterpret_cast<std::uintptr_t>(lua_settop));
}

void LuaExecution::Execute(const int realm, const std::string& script) noexcept {
    if (script.empty())
        return;

    const auto luaInterface = interfaces->luaShared->GetLuaInterface(realm);

    const auto state = *reinterpret_cast<lua_State**>(
        reinterpret_cast<std::uint64_t>(luaInterface) + 0x8);

    if (!state)
        return;

    const auto thread = lua_newthread(state);

    if (!thread)
        return;

    lua_pop(state, 1);

    if (luaL_loadbufferx(thread, script.c_str(), script.length(), "century", 0) == 0) {
        if (lua_pcall(thread, -1, 0, 0) != 0) {
            lua_pop(thread, 1);
        }
    }
    else {
        lua_pop(thread, 1);
    }
}