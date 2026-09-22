//
// Created by sh1rm4n on 6/2/26.
//

#ifndef CENTURY_CCOLLISIONPROPERTY_HPP
#define CENTURY_CCOLLISIONPROPERTY_HPP

#include "../../core/memory.hpp"

class Vector;

class CCollisionProperty {
public:
    constexpr const Vector& OBBMins() noexcept {
        return memory::Call<const Vector&>(this, 3);
    }

    constexpr const Vector& OBBMaxs() noexcept {
        return memory::Call<const Vector&>(this, 4);
    }
};

#endif //CENTURY_CCOLLISIONPROPERTY_HPP
