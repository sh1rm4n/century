//
// Created by sh1rm4n on 7/28/26.
//

#include "ScriptHook.hpp"

#include <filesystem>
#include <fstream>

#include "../core/utils.hpp"

namespace fs = std::filesystem;

void ScriptHook::Dump(const std::string& path, const std::string& content) noexcept {
    const auto netChan = interfaces->engineClient->GetNetChannelInfo();
    if (!netChan)
        return;

    static const fs::path homeDir = std::getenv("HOME");
    const fs::path saveDir = homeDir / "century" / "scripthooks" / netChan->GetAddress();

    fs::create_directories((saveDir / path).parent_path());

    std::ofstream output(saveDir / path);
    output.write(content.data(), static_cast<std::streamsize>(content.size()));
}
