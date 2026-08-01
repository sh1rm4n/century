//
// Created by sh1rm4n on 7/30/26.
//

#ifndef CENTURY_NOSPREAD_HPP
#define CENTURY_NOSPREAD_HPP

#pragma once

#include <memory>

#include "../sourcesdk/math.hpp"

class CUserCmd;

class NoSpread {
private:
    Vector shootDirection = {};
    Vector right = {};
    Vector up = {};
    Vector gunSpread = {};
public:
    void SetSpread(const Vector &spread) noexcept;
    Vector EngineBasedSpread(const Vector &spread, float bias, CUserCmd* cmd) noexcept;
    void Run(CUserCmd* cmd) noexcept;
};

inline auto noSpread = std::make_unique<NoSpread>();

#endif //CENTURY_NOSPREAD_HPP
