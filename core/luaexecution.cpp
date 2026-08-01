//
// Created by sh1rm4n on 6/12/26.
//

#include "luaexecution.hpp"

#include "interfaces.hpp"
#include "modules.hpp"
#include "utils.hpp"

void LuaExecution::Prepare() noexcept {
    lua_newthread = reinterpret_cast<lua_newthread_>(modules->GetExport(modules->luaShared, "lua_newthread"));
    luaL_ref = reinterpret_cast<luaL_ref_>(modules->GetExport(modules->luaShared, "luaL_ref"));
    luaL_unref = reinterpret_cast<luaL_unref_>(modules->GetExport(modules->luaShared, "luaL_unref"));
    luaL_loadbufferx = reinterpret_cast<luaL_loadbufferx_>(modules->GetExport(modules->luaShared, "luaL_loadbufferx"));
    lua_pcall = reinterpret_cast<lua_pcall_>(modules->GetExport(modules->luaShared, "lua_pcall"));
    lua_settop = reinterpret_cast<lua_settop_>(modules->GetExport(modules->luaShared, "lua_settop"));
    lua_gettop = reinterpret_cast<lua_gettop_>(modules->GetExport(modules->luaShared, "lua_gettop"));
    luaL_register = reinterpret_cast<luaL_register_>(modules->GetExport(modules->luaShared, "luaL_register"));
    luaL_checklstring = reinterpret_cast<luaL_checklstring_>(modules->GetExport(modules->luaShared, "luaL_checklstring"));
    lua_tolstring = reinterpret_cast<lua_tolstring_>(modules->GetExport(modules->luaShared, "lua_tolstring"));
    lua_getfield = reinterpret_cast<lua_getfield_>(modules->GetExport(modules->luaShared, "lua_getfield"));
    lua_call = reinterpret_cast<lua_call_>(modules->GetExport(modules->luaShared, "lua_call"));
    lua_pushvalue = reinterpret_cast<lua_pushvalue_>(modules->GetExport(modules->luaShared, "lua_pushvalue"));
    lua_tonumber = reinterpret_cast<lua_tonumber_>(modules->GetExport(modules->luaShared, "lua_tonumber"));
    lua_type = reinterpret_cast<lua_type_>(modules->GetExport(modules->luaShared, "lua_type"));
    lua_pushinteger = reinterpret_cast<lua_pushinteger_>(modules->GetExport(modules->luaShared, "lua_pushinteger"));
    lua_pushnumber = reinterpret_cast<lua_pushnumber_>(modules->GetExport(modules->luaShared, "lua_pushnumber"));
    lua_pushstring = reinterpret_cast<lua_pushstring_>(modules->GetExport(modules->luaShared, "lua_pushstring"));
}

lua_State *LuaExecution::GetLuaState(const int realm) noexcept {
    const auto luaInterface = reinterpret_cast<std::uint64_t>(interfaces->luaShared->GetLuaInterface(realm));

    if (!luaInterface)
        return nullptr;

    const auto state = *reinterpret_cast<lua_State**>(luaInterface + 0x8);

    if (!state)
        return nullptr;

    return state;
}

// century.RunCode(script, chunkName) - Runs script in bypass of RunString(Ex), CompileString, by using luaL_loadbuffer,
// which makes it almost undetectable.
static int RunCode(lua_State* L) {
    const auto code = luaL_checklstring(L, 1);
    const auto chunkName = luaL_checklstring(L, 2);

    luaExecution->LoadBuffer(L, code, chunkName);

    return 0;
}

static int test(lua_State* L) {
    lua_getglobal(L, "FrameTime");

    lua_call(L, 0, 1);

    return 1;
}

static const luaL_Reg customFuncs[] = {
    {"RunCode", RunCode},
    {"test", test},
    {NULL, NULL}
};

void LuaExecution::LoadBuffer(lua_State *state, const std::string &script, const std::string& chunkName) noexcept {
    if (!state || script.empty())
        return;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (luaL_loadbufferx(state, script.c_str(), script.length(), chunkName.c_str(), 0) == 0) {
        if (lua_pcall(state, 0, 0, 0) != 0) {
            const char* error = lua_tostring(state, -1);
            utils::Error("[century] LuaExecution::LoadBuffer calling error: {}", error);

            lua_pop(state, 1);
        }
    }
    else {
        const char* error = lua_tostring(state, -1);
        utils::Error("[century] LuaExecution::LoadBuffer loading error: {}", error);

        lua_pop(state, 1);
    }
}

void LuaExecution::ExecuteInLuaState(lua_State *state, const std::string &script) noexcept {
    static std::unordered_set<lua_State*> loadedEnvironmentStates = {};
    if (!loadedEnvironmentStates.contains(state)) {
        const auto oldTop = lua_gettop(state);

        luaL_register(state, "century", customFuncs);

        lua_settop(state, oldTop);

        loadedEnvironmentStates.emplace(state);
    }

    LoadBuffer(state, script, "");
}

void LuaExecution::Execute(const int realm, const std::string& script) noexcept {
    if (script.empty())
        return;

    ExecuteInLuaState(GetLuaState(realm), script);
}