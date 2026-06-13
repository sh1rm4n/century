//
// Created by sh1rm4n on 5/17/26.
//

#ifndef CENTURY_BUNNYHOP_HPP
#define CENTURY_BUNNYHOP_HPP

#include <memory>

class UserCmd;

enum AutoStrafeType {
    Legit, Directional
};

class BunnyHop {
public:
    void Autostrafe(UserCmd* cmd, AutoStrafeType type) noexcept;
    void Run(UserCmd* cmd) noexcept;
};

inline auto bhop = std::make_unique<BunnyHop>();

#endif //CENTURY_BUNNYHOP_HPP
