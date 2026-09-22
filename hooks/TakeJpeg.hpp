//
// Created by sh1rm4n on 6/7/26.
//

#ifndef CENTURY_TAKEJPEG_HPP
#define CENTURY_TAKEJPEG_HPP

#pragma once

#include <polyhook2/Detour/x64Detour.hpp>

#include "../3rd/imgui/imgui_notify/ImGuiNotify.hpp"

inline std::uint64_t ogTakeJpeg = 0;

void dtTakeJpeg(const char* name, int quality) {
    static auto ogFunction = PLH::FnCast(ogTakeJpeg, dtTakeJpeg);

    ImGui::InsertNotification({ImGuiToastType::Warning, "Prevented attempt to screengrab! (Type: 2)"});

    interfaces->engineClient->ClientCmd_Unrestricted("play hl1/fvox/biohazard_detected.wav");

    return;
}

#endif //CENTURY_TAKEJPEG_HPP
