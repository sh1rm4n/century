//
// Created by sh1rm4n on 7/9/26.
//

#ifndef CENTURY_ANTIAIM_HPP
#define CENTURY_ANTIAIM_HPP

#pragma once

class UserCmd;

class AntiAim {
public:
    void Run(UserCmd* cmd) noexcept;
};

inline auto antiAim = std::make_unique<AntiAim>();

#endif //CENTURY_ANTIAIM_HPP
