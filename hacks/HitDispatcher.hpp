//
// Created by sh1rm4n on 8/26/26.
//

#ifndef CENTURY_HITDISPATCHER_HPP
#define CENTURY_HITDISPATCHER_HPP

#pragma once

#include <memory>

enum class MissTypes {
    Resolver,
    BackTracking,
};

enum class RequestTypes {
    Aimbot
};

// aimbot tells hitdispatcher for hit await, hitdispatcher tells aimbot that it missed or not

class HitDispatcher {
private:
    std::vector<RequestTypes> waitingList;
public:
    void AimbotHit() noexcept;
    void Reply(RequestTypes replyTo, MissTypes missType) noexcept;
    void Run() noexcept;
};

inline auto hitDispatcher = std::make_unique<HitDispatcher>();

#endif //CENTURY_HITDISPATCHER_HPP
