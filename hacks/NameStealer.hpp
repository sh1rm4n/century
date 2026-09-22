//
// Created by sh1rm4n on 8/1/26.
//

#ifndef CENTURY_NAMESTEALER_HPP
#define CENTURY_NAMESTEALER_HPP

#pragma once

#include <memory>
#include <vector>

class NameStealer {
private:
    std::vector<std::string> names;
public:
    void AddName(const std::string &name) noexcept;
    bool IsNameAdded(const std::string &name) noexcept;
    void Run() noexcept;
};

inline auto nameStealer = std::make_unique<NameStealer>();

#endif //CENTURY_NAMESTEALER_HPP
