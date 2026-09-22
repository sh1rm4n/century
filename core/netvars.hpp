//
// Created by sh1rm4n on 7/8/26.
//

#ifndef CENTURY_NETVARS_HPP
#define CENTURY_NETVARS_HPP

#pragma once

class RecvTable;

class NetVars {
private:
    std::unordered_map<std::string, std::uint32_t> offsets;
public:
    void Prepare() noexcept;
    void Dump(const std::string &className, RecvTable* table, std::uint32_t offset = 0) noexcept;
    std::uint32_t GetOffset(const std::string &value) noexcept;
};

inline auto netVars = std::make_shared<NetVars>();

#endif //CENTURY_NETVARS_HPP
