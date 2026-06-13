//
// Created by sh1rm4n on 6/9/26.
//

#ifndef CENTURY_AIMBOT_HPP
#define CENTURY_AIMBOT_HPP

#include "../../../core/globals.hpp"

#include "../../../3rd/imgui/imgui_toggle/imgui_toggle.h"

namespace menu::pages::aimbot {
    void Render() noexcept {
        ImGui::ToggleAnimated("Enable aimbot?", &globals::hacks::aimbot::enabled);
    }
}

#endif // CENTURY_AIMBOT_HPP
