//
// Created by sh1rm4n on 8/1/26.
//

#include "NameStealer.hpp"

#include "../core/interfaces.hpp"
#include "../core/utils.hpp"
#include "../sourcesdk/interfaces/CConVar.hpp"

void NameStealer::AddName(const std::string &name) noexcept {
    if (name.empty())
        return;

    names.emplace_back(name);
}

bool NameStealer::IsNameAdded(const std::string &name) noexcept {
    if (name.empty())
        return true;

    for (auto& n : names) {
        if (n == name)
            return true;
    }

    return false;
}

void NameStealer::Run() noexcept {
    if (names.empty())
        return;

    auto name = names[utils::Random(0, static_cast<int>(names.size() - 1))];

    const std::unordered_map<char, std::string> charsToReplace = {
        {'A', "А"},
        {'B', "В"},
        {'C', "С"},
        {'E', "Е"},
        {'H', "Н"},
        {'K', "К"},
        {'M', "М"},
        {'O', "О"},
        {'P', "Р"},
        {'T', "Т"},
        {'X', "Х"},
        {'Y', "У"},

        {'a', "а"},
        {'c', "с"},
        {'e', "е"},
        {'o', "о"},
        {'p', "р"},
        {'x', "х"},
        {'y', "у"},
    };

    std::string result;
    result.reserve(name.size() * 2);

    for (const char c : name) {
        if (const auto it = charsToReplace.find(c); it != charsToReplace.end())
            result += it->second;
        else
            result += c;
    }

    name = std::move(result);

    interfaces->engineClient->ClientCmd_Unrestricted(
        fmt::format("name {}", name).c_str());
}