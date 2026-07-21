//
// Created by sh1rm4n on 7/9/26.
//

#ifndef CENTURY_RESOLVER_HPP
#define CENTURY_RESOLVER_HPP

#pragma once

class C_BaseEntity;
class Vector;

class Bruteforce {
public:
    Vector Brute(C_BaseEntity* player, Vector bonePos) noexcept;
};

inline auto bruteForcer = std::make_unique<Bruteforce>();

#endif //CENTURY_RESOLVER_HPP
