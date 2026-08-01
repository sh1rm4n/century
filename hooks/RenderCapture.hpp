//
// Created by sh1rm4n on 6/7/26.
//

#ifndef CENTURY_RENDERCAPTURE_HPP
#define CENTURY_RENDERCAPTURE_HPP

#pragma once

#include <polyhook2/Detour/x64Detour.hpp>

inline std::uint64_t ogRenderCapture = 0;

int dtRenderCapture(void* luaState) {
    static auto ogFunction = PLH::FnCast(ogRenderCapture, dtRenderCapture);

    ImGui::InsertNotification({ImGuiToastType::Warning, "Prevented attempt to screengrab! (Type: 3)"});

    interfaces->engineClient->ClientCmd_Unrestricted("play hl1/fvox/biohazard_detected.wav");

    return 1;
}

#endif //CENTURY_RENDERCAPTURE_HPP
