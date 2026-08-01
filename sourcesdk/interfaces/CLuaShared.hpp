//
// Created by sh1rm4n on 6/13/26.
//

#ifndef CENTURY_CLUASHARED_HPP
#define CENTURY_CLUASHARED_HPP

#include <cstdint>

class CLuaInterface;

class CLuaShared {
public:
    constexpr CLuaInterface* GetLuaInterface(const int realm) noexcept {
#if defined(__linux__)
        return memory::Call<CLuaInterface*>(this, 7, realm);
#else
        return memory::Call<CLuaInterface*>(this, 6, realm);
#endif
    }
};

#endif //CENTURY_CLUASHARED_HPP
