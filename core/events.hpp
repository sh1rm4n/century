//
// Created by sh1rm4n on 7/9/26.
//

#ifndef CENTURY_EVENTS_HPP
#define CENTURY_EVENTS_HPP

#pragma once

#include <memory>

class IGameEventListener2;

class Events {
private:
    IGameEventListener2* playerHurt;
    IGameEventListener2* playerDeath;
public:
    void Prepare() noexcept;
    void Unload() noexcept;
};

inline auto events = std::make_shared<Events>();

#endif //CENTURY_EVENTS_HPP
