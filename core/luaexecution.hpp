//
// Created by sh1rm4n on 6/12/26.
//

#ifndef CENTURY_LUAEXECUTION_HPP
#define CENTURY_LUAEXECUTION_HPP

#pragma once

typedef struct lua_State lua_State;

typedef int (*lua_CFunction) (lua_State *L);

typedef struct luaL_Reg {
    const char *name;
    lua_CFunction func;
} luaL_Reg;

using lua_newthread_ = lua_State*(*)(lua_State*);
inline lua_newthread_ lua_newthread = nullptr;

using luaL_ref_ = int(*)(lua_State*, int);
inline luaL_ref_ luaL_ref = nullptr;

using luaL_unref_ = void(*)(lua_State*, int, int);
inline luaL_unref_ luaL_unref = nullptr;

using luaL_loadbufferx_ = int(*)(lua_State*, const char*, size_t, const char*, int);
inline luaL_loadbufferx_ luaL_loadbufferx = nullptr;

using lua_pcall_ = int(*)(lua_State*, int, int, int);
inline lua_pcall_ lua_pcall = nullptr;

using lua_settop_ = void(*)(lua_State*, int);
inline lua_settop_ lua_settop = nullptr;

using lua_gettop_ = int(*)(lua_State*);
inline lua_gettop_ lua_gettop = nullptr;

using luaL_register_ = void(*)(lua_State*, const char*, const luaL_Reg*);
inline luaL_register_ luaL_register = nullptr;

using luaL_checklstring_ = const char*(*)(lua_State*, int);
inline luaL_checklstring_ luaL_checklstring = nullptr;

using lua_tolstring_ = const char*(*)(lua_State*, int, size_t);
inline lua_tolstring_ lua_tolstring = nullptr;

using lua_getfield_ = void(*)(lua_State*, int, const char*);
inline lua_getfield_ lua_getfield = nullptr;

using lua_call_ = void(*)(lua_State*, int, int);
inline lua_call_ lua_call = nullptr;

using lua_pushvalue_ = void(*)(lua_State*, int);
inline lua_pushvalue_ lua_pushvalue = nullptr;

using lua_tonumber_ = double(*)(lua_State*, int);
inline lua_tonumber_ lua_tonumber = nullptr;

using lua_type_ = int(*)(lua_State*, int);
inline lua_type_ lua_type = nullptr;

using lua_pushinteger_ = void(*)(lua_State*, int);
inline lua_pushinteger_ lua_pushinteger = nullptr;

using lua_pushnumber_ = void(*)(lua_State*, double);
inline lua_pushnumber_ lua_pushnumber = nullptr;

using lua_pushstring_ = void(*)(lua_State*, const char*);
inline lua_pushstring_ lua_pushstring = nullptr;

using lua_remove_ = void(*)(lua_State*, int);
inline lua_remove_ lua_remove = nullptr;

enum {
    LUA_GLOBALSINDEX = -10002,
    LUA_ENVIRONINDEX = -10001,
    LUA_REGISTRYINDEX = -10000
};

enum {
    LUA_TNONE = -1,
    LUA_TNIL = 0,
    LUA_TBOOLEAN = 1,
    LUA_TLIGHTUSERDATA = 2,
    LUA_TNUMBER = 3,
    LUA_TSTRING = 4,
    LUA_TTABLE = 5,
    LUA_TFUNCTION = 6,
    LUA_TUSERDATA = 7,
    LUA_TTHREAD = 8
};

#define lua_pop(L,n) lua_settop(L, -(n)-1)
#define lua_getglobal(L,s)  lua_getfield(L, LUA_GLOBALSINDEX, s)
#define lua_tostring(L,i) lua_tolstring(L, (i), NULL)

#define lua_isstring(L,n) (lua_type(L, (n)) == LUA_TSTRING)
#define lua_isnumber(L,n) (lua_type(L, (n)) == LUA_TNUMBER)
#define lua_istable(L,n) (lua_type(L, (n)) == LUA_TTABLE)
#define lua_isfunction(L,n) (lua_type(L, (n)) == LUA_TFUNCTION)
#define lua_islightuserdata(L,n) (lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n) (lua_type(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n) (lua_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n) (lua_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n) (lua_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L,n) (lua_type(L, (n)) <= 0)

class LuaExecution {
public:
    void Prepare() noexcept;
    lua_State* GetLuaState(const int realm) noexcept;
    void LoadBuffer(lua_State* state, const std::string& script, const std::string& chunkName) noexcept;
    void ExecuteInLuaState(lua_State* state, const std::string& script) noexcept;
    void Execute(const int realm, const std::string& script) noexcept;
};

inline auto luaExecution = std::make_shared<LuaExecution>();

#endif //CENTURY_LUAEXECUTION_HPP
