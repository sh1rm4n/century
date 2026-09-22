//
// Created by sh1rm4n on 7/10/26.
//

#ifndef CENTURY_FAKELAG_HPP
#define CENTURY_FAKELAG_HPP

#pragma once

#include <memory>

#include "../sourcesdk/math.hpp"

class FakeLag {
private:
    Vector lastServerPos;
    double lastServerSimulationTime;
    std::array<Matrix3x4, 256> lastServerMatrix;
public:
    void Run(int ticks) noexcept;
    Vector GetLastServerPos() noexcept;
    double GetLastServerSimulationTime() noexcept;
    std::array<Matrix3x4, 256> GetLastServerMatrix() noexcept;
};

inline auto fakeLag = std::make_unique<FakeLag>();

#endif //CENTURY_FAKELAG_HPP
