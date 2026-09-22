//
// Created by sh1rm4n on 7/28/26.
//

#ifndef CENTURY_SCRIPTHOOK_HPP
#define CENTURY_SCRIPTHOOK_HPP

#include <memory>

class ScriptHook {
public:
    void Dump(const std::string& path, const std::string& content) noexcept;
};

inline auto scriptHook = std::make_unique<ScriptHook>();

#endif //CENTURY_SCRIPTHOOK_HPP
