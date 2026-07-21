//
// Created by sh1rm4n on 7/10/26.
//

#ifndef CENTURY_FAKELAG_HPP
#define CENTURY_FAKELAG_HPP

#pragma once

#include <memory>

class FakeLag {
public:
    void Run(int ticks) noexcept;
};

inline auto fakeLag = std::make_unique<FakeLag>();

#endif //CENTURY_FAKELAG_HPP
