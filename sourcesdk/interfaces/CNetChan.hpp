//
// Created by sh1rm4n on 6/5/26.
//

#ifndef CENTURY_CNETCHAN_HPP
#define CENTURY_CNETCHAN_HPP

#include "../../core/memory.hpp"

class CNetChan
{
public:
    constexpr void Shutdown(const char* reason) noexcept {
        memory::Call<void>(this, 37, reason);
    }
};

#endif //CENTURY_CNETCHAN_HPP
