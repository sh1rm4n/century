//
// Created by sh1rm4n on 7/4/26.
//

#ifndef CENTURY_SHOULDDRAWLOCALPLAYER_HPP
#define CENTURY_SHOULDDRAWLOCALPLAYER_HPP

#pragma once

#include <polyhook2/Detour/x64Detour.hpp>

class C_BaseEntity;

inline std::uint64_t ogShouldDrawLocalPlayer = 0;

bool dtShouldDrawLocalPlayer(C_BaseEntity* vmt) {
    return false;
}

#endif //CENTURY_SHOULDDRAWLOCALPLAYER_HPP
