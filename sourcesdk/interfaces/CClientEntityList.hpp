//
// Created by sh1rm4n on 5/18/26.
//

#ifndef CENTURY_ICLIENTENTITYLIST_HPP
#define CENTURY_ICLIENTENTITYLIST_HPP
#include "../../core/memory.hpp"

class CClientEntityList {
public:
    template <typename Entity>
    constexpr Entity* GetClientEntity(const int index) noexcept {
        return memory::Call<Entity*>(this, 3, index);
    }

    constexpr int GetMaxEntities() noexcept {
        return memory::Call<int>(this, 8);
    }

    constexpr int GetHighestEntityIndex() noexcept {
        return memory::Call<int>(this, 10);
    }
};

#endif //CENTURY_ICLIENTENTITYLIST_HPP
