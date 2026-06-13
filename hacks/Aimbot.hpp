//
// Created by sh1rm4n on 6/9/26.
//

#ifndef CENTURY_AIMBOT_HACK_HPP
#define CENTURY_AIMBOT_HACK_HPP

#include <memory>

#include "../sourcesdk/UserCmd.hpp"

class Aimbot {
public:
    void Run(UserCmd* cmd) noexcept;
};

inline auto aimbot = std::make_unique<Aimbot>();

#endif //CENTURY_AIMBOT_HACK_HPP
