//
// Created by sh1rm4n on 6/7/26.
//

#ifndef CENTURY_TAKESCREENSHOT_HPP
#define CENTURY_TAKESCREENSHOT_HPP

#pragma once

#include <polyhook2/Detour/x64Detour.hpp>

#include "../3rd/imgui/imgui_notify/ImGuiNotify.hpp"

inline std::uint64_t ogTakeScreenshot = 0;

void dtTakeScreenshot(const char* name) {
    static auto ogFunction = PLH::FnCast(ogTakeScreenshot, dtTakeScreenshot);

    ImGui::InsertNotification({ImGuiToastType::Warning, "Prevented attempt to screengrab! (Type: 1)"});

    interfaces->engineClient->ClientCmd_Unrestricted("play hl1/fvox/biohazard_detected.wav");

    return;
}

#endif //CENTURY_TAKESCREENSHOT_HPP
