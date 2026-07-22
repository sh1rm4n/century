//
// Created by sh1rm4n on 6/12/26.
//

#include "luaexecution.hpp"

#include "interfaces.hpp"
#include "modules.hpp"

#define lua_pop(L,n) lua_settop(L, -(n)-1)

void LuaExecution::Prepare() noexcept {
    lua_newthread = reinterpret_cast<lua_newthread_>(modules->GetExport(modules->luaShared, "lua_newthread"));
    luaL_ref = reinterpret_cast<luaL_ref_>(modules->GetExport(modules->luaShared, "luaL_ref"));
    luaL_unref = reinterpret_cast<luaL_unref_>(modules->GetExport(modules->luaShared, "luaL_unref"));
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

    if (!luaInterface)
        return;

    const auto state = *reinterpret_cast<lua_State**>(
        reinterpret_cast<std::uint64_t>(luaInterface) + 0x8);

    if (!state)
        return;

    const auto thread = lua_newthread(state);

    if (!thread)
        return;

    const auto threadRef = luaL_ref(state, -10000);

    if (!threadRef)
        return;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (luaL_loadbufferx(thread, script.c_str(), script.length(), "@century", 0) == 0) {
        if (lua_pcall(thread, 0, 0, 0) != 0) {
            lua_pop(thread, 1);
        }
    }
    else {
        lua_pop(thread, 1);
    }

    luaL_unref(state, -10000, threadRef);
}