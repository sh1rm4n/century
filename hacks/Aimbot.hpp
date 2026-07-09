#ifndef CENTURY_AIMBOT_HACK_HPP
#define CENTURY_AIMBOT_HACK_HPP

#pragma once

class UserCmd;

class Aimbot {
public:
    void Run(UserCmd* cmd) noexcept;
};

inline auto aimbot = std::make_unique<Aimbot>();

#endif
