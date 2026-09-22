#ifndef CENTURY_AIMBOT_HACK_HPP
#define CENTURY_AIMBOT_HACK_HPP

#pragma once

class CUserCmd;

class Aimbot {
public:
    void Run(CUserCmd* cmd) noexcept;
};

inline auto aimbot = std::make_unique<Aimbot>();

#endif
