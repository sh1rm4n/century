//
// Created by sh1rm4n on 7/28/26.
//

#ifndef CENTURY_CLUAINTERFACE_HPP
#define CENTURY_CLUAINTERFACE_HPP

class Angle;
class Vector;

namespace Type {
    enum {
        INVALID = -1,
        NIL,
        BOOL,
        LIGHTUSERDATA,
        NUMBER,
        STRING,
        TABLE,
        FUNCTION,
        USERDATA,
        THREAD,

        // UserData
        ENTITY,
        VECTOR,
        ANGLE,
        PHYSOBJ,
        SAVE,
        RESTORE,
        DAMAGEINFO,
        EFFECTDATA,
        MOVEDATA,
        RECIPIENTFILTER,
        USERCMD,
        SCRIPTEDVEHICLE,

        // Client Only
        MATERIAL,
        PANEL,
        PARTICLE,
        PARTICLEEMITTER,
        TEXTURE,
        USERMSG,

        CONVAR,
        IMESH,
        MATRIX,
        SOUND,
        PIXELVISHANDLE,
        DLIGHT,
        VIDEO,
        FILE,

        COUNT
    };

    enum
    {
        SPECIAL_GLOB,       // Global table
        SPECIAL_ENV,        // Environment table
        SPECIAL_REG,        // Registry table
    };

    static const char* Name[] = {
        "nil",
        "bool",
        "lightuserdata",
        "number",
        "string",
        "table",
        "function",
        "userdata",
        "thread",
        "entity",
        "vector",
        "angle",
        "physobj",
        "save",
        "restore",
        "damageinfo",
        "effectdata",
        "movedata",
        "recipientfilter",
        "usercmd",
        "vehicle",
        "material",
        "panel",
        "particle",
        "particleemitter",
        "texture",
        "usermsg",
        "convar",
        "mesh",
        "matrix",
        "sound",
        "pixelvishandle",
        "dlight",
        "video",
        "file",

        0
    };
}

class CLuaInterface {
public:
    virtual int Pop(int) = 0; // 0
    virtual void GetTable(int) = 0; // 1
    virtual void GetField(int,char const*) = 0; // 2
    virtual void SetField(int,char const*) = 0; // 3
    virtual int CreateTable(void) = 0; // 4
    virtual int SetTable(int) = 0; // 5
    virtual int SetMetaTable(int) = 0; // 6
    virtual int GetMetaTable(int) = 0; // 7
    virtual int Call(int,int) = 0; // 8
    virtual int PCall(int,int,int) = 0; // 9
    virtual int Equal(int,int) = 0; // 10
    virtual int RawEqual(int,int) = 0; // 11
    virtual int Insert(int) = 0; // 12
    virtual int Remove(int) = 0; // 13
    virtual int Next(int) = 0; // 14
    virtual int NewUserdata(uint) = 0; // 15
    virtual int ThrowError(char const*) = 0; // 16
    virtual int CheckType(int,int) = 0; // 17
    virtual int ArgError(int,char const*) = 0; // 18
    virtual int RawGet(int) = 0; // 19
    virtual int RawSet(int) = 0; // 20
    virtual const char* GetString(int, int) = 0; // 21
    virtual int GetNumber(int) = 0; // 22
    virtual int GetBool(int) = 0; // 23
    virtual int GetCFunction(int) = 0; // 24
    virtual int GetUserdata(int) = 0; // 25
    virtual int PushNil(void) = 0; // 26
    virtual void PushString(char const*,std::uint32_t) = 0; // 27
    virtual void PushNumber(double) = 0; // 28
    virtual void PushBool(bool) = 0; // 29
    virtual void PushCFunction(int (*)(lua_State *)) = 0; // 30
    virtual void PushCClosure(int (*)(lua_State *),int) = 0; // 31
    virtual void PushUserdata(void *) = 0; // 32
    virtual void ReferenceCreate(void) = 0; // 33
    virtual void ReferenceFree(int) = 0; // 34
    virtual void ReferencePush(int) = 0; // 35
    virtual void PushSpecial(int) = 0; // 36
    virtual bool IsType(int,int) = 0; // 37
    virtual void GetType(int) = 0; // 38
    virtual void GetTypeName(int) = 0; // 39
    virtual void CreateMetaTableType(char const*,int) = 0; // 40
    virtual void CheckString(int) = 0; // 41
    virtual void CheckNumber(int) = 0; // 42
    virtual int ObjLen(int) = 0; // 43
    virtual int GetAngle(int) = 0; // 44
    virtual int GetVector(int) = 0; // 45
    virtual void PushAngle(Angle const&) = 0; // 46
    virtual void PushVector(Vector const&) = 0; // 47
    virtual void SetState(lua_State *) = 0; // 48
    virtual int CreateMetaTable(char const*) = 0; // 49
    virtual int PushMetaTable(int) = 0; // 50
    virtual int PushUserType(void *,int) = 0; // 51
    virtual int SetUserType(int,void *) = 0; // 52
    virtual int Init(void**,bool) = 0; // 53
    virtual void Shutdown(void) = 0; // 54
    virtual void Cycle(void) = 0; // 55
    virtual void Global(void) = 0; // 56
    virtual int GetObject(int) = 0; // 57
    virtual void PushLuaObject(void**) = 0; // 58
    virtual void PushLuaFunction(int (*)(lua_State *)) = 0; // 59
    virtual void LuaError(char const*,int) = 0; // 60
    virtual void TypeError(char const*,int) = 0; // 61
    virtual void CallInternal(int,int) = 0; // 62
    virtual int CallInternalNoReturns(int) = 0; // 63
    virtual void CallInternalGetBool(int) = 0; // 64
    virtual void CallInternalGetString(int) = 0; // 65
    virtual void CallInternalGet(int,void**) = 0; // 66
    virtual void NewGlobalTable(char const*) = 0; // 67
    virtual int NewTemporaryObject(void) = 0; // 68
    virtual int isUserData(int) = 0; // 69
    virtual void GetMetaTableObject(char const*,int) = 0; // 70
    virtual void GetMetaTableObject(int) = 0; // 71
    virtual int GetReturn(int) = 0; // 72
    virtual void IsServer(void) = 0; // 73
    virtual void IsClient(void) = 0; // 74
    virtual void IsMenu(void) = 0; // 75
    virtual void DestroyObject(void* *) = 0; // 76
    virtual void CreateObject(void) = 0; // 77
    virtual void SetMember(void* *,void* *,void* *) = 0; // 78
    virtual void GetNewTable(void) = 0; // 79
    virtual void SetMember(void* *,float) = 0; // 80
    virtual void SetMember(void* *,float,void* *) = 0; // 81
    virtual void SetMember(void* *,char const*) = 0; // 82
    virtual void SetMember(void* *,char const*,void* *) = 0; // 83
    virtual int SetType(std::uint8_t) = 0; // 84
    virtual int PushLong(long) = 0; // 85
    virtual int GetFlags(int) = 0; // 86
    virtual void FindOnObjectsMetaTable(int,int) = 0; // 87
    virtual void FindObjectOnTable(int,int) = 0; // 88
    virtual void SetMemberFast(void* *,int,int) = 0; // 89
    virtual void RunString(char const*,char const*,char const*,bool,bool) = 0; // 90
    virtual void IsEqual(void* *,void* *) = 0; // 91
    virtual void Error(char const*) = 0; // 92
    virtual void GetStringOrError(int) = 0; // 93
    virtual int RunLuaModule(char const*) = 0; // 94
    virtual int FindAndRunScript(char const*,bool,bool,char const*,bool) = 0; // 95
    virtual int SetPathID(char const*) = 0; // 96
    virtual int GetPathID(void) = 0; // 97
    virtual int ErrorNoHalt(char const*,...) = 0; // 98
    virtual int Msg(char const*,...) = 0; // 99
    virtual int PushPath(char const*) = 0; // 100
    virtual int PopPath(void) = 0; // 101
    virtual int GetPath(void) = 0; // 102
    virtual void GetColor(int) = 0; // 103
    virtual void PushColor(Color) = 0; // 104
    virtual int GetStack(int,void *) = 0; // 105
    virtual int GetInfo(char const*,void *) = 0; // 106
    virtual int GetLocal(void *,int) = 0; // 107
    virtual void GetUpvalue(int,int) = 0; // 108
    virtual int RunStringEx(char const*,char const*,char const*,bool,bool,bool,bool) = 0; // 109
    virtual void GetDataString(int,void **) = 0; // 110
    virtual int ErrorFromLua(char const*,...) = 0; // 111
    virtual void GetCurrentLocation(void) = 0; // 112
    virtual void MsgColour(Color const&,char const*,...) = 0; // 113
    virtual void GetCurrentFile(std::string&) = 0; // 114
    virtual void CompileString(void*&, std::string const&) = 0; // 115
    virtual void CallFunctionProtected(int,int,bool) = 0; // 116
    virtual void Require(char const*) = 0; // 117
    virtual void GetActualTypeName(int) = 0; // 118
    virtual int PreCreateTable(int,int) = 0; // 119
    virtual void PushPooledString(int) = 0; // 120
    virtual int GetPooledString(int) = 0; // 121
    virtual void AddThreadedCall(void**) = 0; // 122
    virtual int AppendStackTrace(char *,ulong) = 0; // 123
    virtual int CreateConVar(char const*,char const*,char const*,int) = 0; // 124
    virtual void CreateConCommand(char const*,char const*,int,void (*)(void* const&),int (*)(char const*,char (*)[128])) = 0; // 125
    virtual int CheckStringOpt(int,char const*) = 0; // 126
    virtual int CheckNumberOpt(int,double) = 0; // 127
    virtual void RegisterMetaTable(char const*,void**) = 0; // 128

    constexpr bool GetRealm() noexcept {
        return *reinterpret_cast<bool*>(std::uint64_t(this) + 0x168);
    }
};

#endif //CENTURY_CLUAINTERFACE_HPP
