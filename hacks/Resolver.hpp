//
// Created by sh1rm4n on 7/9/26.
//

#ifndef CENTURY_RESOLVER_HPP
#define CENTURY_RESOLVER_HPP

#pragma once

class C_BaseEntity;
class Vector;

class Resolver {
private:
    std::unordered_set<C_BaseEntity*> resolvingPlayers;
public:
    void Resolve(C_BaseEntity* player, int shotsMade) noexcept;
    void RemovePlayer(C_BaseEntity* player) noexcept;
    bool ContainsPlayer(C_BaseEntity* player) noexcept;
};

inline auto resolver = std::make_unique<Resolver>();

#endif //CENTURY_RESOLVER_HPP
