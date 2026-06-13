//
// Created by sh1rm4n on 6/13/26.
//

#ifndef CENTURY_CLUASHARED_HPP
#define CENTURY_CLUASHARED_HPP

#include <string>
#include <vector>

enum LuaInterfaceType
{
    LUA_CLIENT = 0,
    LUA_SERVER = 1,
    LUA_MENU = 2
};

class CLuaShared
{
public:
    virtual void constructor() = 0; // 0
    virtual void destructor() = 0; // 1
    virtual char Init(void * (*)(char const*,int *), bool, void*) = 0; // 2
    virtual int Shutdown(void) = 0; // 3
    virtual int DumpStats(void) = 0; // 4
    virtual int CreateLuaInterface(bool,bool) = 0; // 5
    virtual void CloseLuaInterface(void *) = 0; // 6
    virtual void* GetLuaInterface(LuaInterfaceType) = 0; // 7
    virtual int LoadFile(std::string const&,std::string const&,bool,bool) = 0; // 8
    virtual void GetCache(std::string const&) = 0; // 9
    virtual int MountLua(char const*) = 0; // 10
    virtual int MountLuaAdd(char const*,char const*) = 0; // 11
    virtual int UnMountLua(char const*) = 0; // 12
    virtual int SetFileContents(char const*,char const*) = 0; // 13
    virtual void SetLuaFindHook(void *) = 0; // 14
    virtual void FindScripts(std::string const&,std::string const&,std::vector<void*> &) = 0; // 15
    virtual int GetStackTraces(void) = 0; // 16
    virtual void InvalidateCache(std::string const&) = 0; // 17
    virtual int EmptyCache(void) = 0; // 18
    virtual void ScriptExists(std::string const&,std::string const&,bool) = 0; // 19
};
// 20 virtual functions

#endif //CENTURY_CLUASHARED_HPP
