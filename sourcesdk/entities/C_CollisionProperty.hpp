//
// Created by sh1rm4n on 7/23/26.
//

#ifndef CENTURY_C_COLLISIONPROPERTY_HPP
#define CENTURY_C_COLLISIONPROPERTY_HPP

#include "../../core/utils.hpp"

class Vector;

class C_CollisionProperty {
public:
    constexpr Vector& OBBMins() noexcept {
        return *reinterpret_cast<Vector*>(std::uint64_t(this) + utils::NetVar("DT_CollisionProperty", "m_vecMins"));
    }

    constexpr Vector& OBBMaxs() noexcept {
        return *reinterpret_cast<Vector*>(std::uint64_t(this) + utils::NetVar("DT_CollisionProperty", "m_vecMaxs"));
    }
};

#endif //CENTURY_C_COLLISIONPROPERTY_HPP
