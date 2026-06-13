//
// Created by sh1rm4n on 6/3/26.
//

#ifndef CENTURY_C_GMOD_PLAYER_HPP
#define CENTURY_C_GMOD_PLAYER_HPP

#include <cstdint>

#include "C_BaseEntity.hpp"

class Angle;

class C_GMOD_Player : C_BaseEntity {
public:
    constexpr void ViewPunch(const Angle& angle) {
        //*reinterpret_cast<Angle*>(std::uint64_t(this) + 0x2A58) = angle;
    }
};

#endif //CENTURY_C_GMOD_PLAYER_HPP
