//
// Created by sh1rm4n on 6/20/26.
//

#ifndef CENTURY_CCONVAR_HPP
#define CENTURY_CCONVAR_HPP

#include <cstdint>

#pragma once

using FnChangeCallback = void(*)(CConVar *var, const char *pOldValue, float flOldValue);

class CConVar {
public:
    CConVar* pNextConvar; //0x0008
    uint32_t bRegistered; //0x0010
    char pad_0014[4]; //0x0014
    char *pszName; //0x0018
    char *pszHelpString; //0x0020
    uint32_t flags; //0x0028
    char pad_002C[4]; //0x002C
    void *s_pConCommandBases; //0x0030
    CConVar *pParent; //0x0038
    char *pszDefaultValue; //0x0040
    char *pszValueStr; //0x0048
    uint32_t strLength; //0x0050
    float fVal; //0x0054
    int32_t intValue; //0x0058
    uint32_t bHasMin; //0x005C
    float fMinVal; //0x0060
    uint32_t bHasMax; //0x0064
    float fMaxVal; //0x0068
    int unknown; //0x006C
    FnChangeCallback callbackPtr; //0x0070
public:
    virtual void constructor() = 0; // 0
    virtual void destructor() = 0; // 1
    virtual int IsCommand(void) = 0; // 2
    virtual int IsFlagSet(int) = 0; // 3
    virtual int AddFlags(int) = 0; // 4
    virtual int GetName(void) = 0; // 5
    virtual int GetHelpText(void) = 0; // 6
    virtual int IsRegistered(void) = 0; // 7
    virtual int GetDLLIdentifier(void) = 0; // 8
    virtual int CreateBase(char const*,char const*,int) = 0; // 9
    virtual int Init(void) = 0; // 10
    virtual int SetValue(char const*) = 0; // 11
    virtual int SetValue(float) = 0; // 12
    virtual int SetValue(int) = 0; // 13
    virtual int InternalSetValue(char const*) = 0; // 14
    virtual int InternalSetFloatValue(float) = 0; // 15
    virtual int InternalSetIntValue(int) = 0; // 16
    virtual int ClampValue(float &) = 0; // 17
    virtual int ChangeStringValue(char const*,float) = 0; // 18
    virtual void Create(char const*,char const*,int,char const*,bool,float,bool,float,void (*)(CConVar *,char const*,float)) = 0; // 19
    virtual int SetServerValue(char const*) = 0; // 20
    virtual int GetFlags(void) = 0; // 21

    constexpr void RemoveFlags(int flags) noexcept {
        this->flags &= ~flags;
    }

    constexpr void ChangeCallback(FnChangeCallback callback) noexcept {
        *reinterpret_cast<FnChangeCallback*>(callbackPtr) = callback;
    }

    constexpr float GetFloat() noexcept {
        return std::stof(pszValueStr);
    }

    constexpr int GetInt() noexcept {
        return std::stoi(pszValueStr);
    }
};
// 22 virtual functions

#endif //CENTURY_CCONVAR_HPP
