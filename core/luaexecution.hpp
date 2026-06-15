//
// Created by sh1rm4n on 6/12/26.
//

#ifndef CENTURY_LUAEXECUTION_HPP
#define CENTURY_LUAEXECUTION_HPP

#include <memory>

#include "../sourcesdk/interfaces/CLuaShared.hpp"

typedef struct lua_State lua_State;

class LuaExecution {
private:
    using lua_newthread_ = lua_State*(*)(lua_State*);
    lua_newthread_ lua_newthread = nullptr;

    using luaL_loadbufferx_ = int(*)(lua_State*, const char*, size_t, const char*, int);
    luaL_loadbufferx_ luaL_loadbufferx = nullptr;

    using lua_pcall_ = int(*)(lua_State*, int, int, int);
    lua_pcall_ lua_pcall = nullptr;

    using lua_settop_ = void(*)(lua_State*, int);
    lua_settop_ lua_settop = nullptr;
public:
    void Prepare() noexcept;
    void Execute(const int realm, const std::string& script) noexcept;
};

inline auto luaExecution = std::make_shared<LuaExecution>();

#endif //CENTURY_LUAEXECUTION_HPP
