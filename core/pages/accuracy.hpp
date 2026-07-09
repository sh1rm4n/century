//
// Created by sh1rm4n on 6/9/26.
//

#ifndef CENTURY_AIMBOT_HPP
#define CENTURY_AIMBOT_HPP

#include "../globals.hpp"

#include "../../3rd/imgui/imgui_toggle/imgui_toggle.h"

namespace menu::pages::accuracy {
    void Render() noexcept {
        if (ImGui::BeginChild("##Aimbot", ImVec2(0, 0), ImGuiChildFlags_Borders)) {
            ImGui::SeparatorText("Aimbot");
            ImGui::ToggleAnimated("Enabled", &globals::hacks::aimbot::enabled);
            ImGui::ToggleAnimated("Silent aim", &globals::hacks::aimbot::silentAim);
            ImGui::ToggleAnimated("pSilent aim", &globals::hacks::aimbot::perfectSilent);
            ImGui::ToggleAnimated("Team checking", &globals::hacks::aimbot::teamCheck);
            ImGui::SliderFloat("Field-of-View", &globals::hacks::aimbot::fov, 1.0f, 180.0f);
            ImGui::SliderFloat("Smoothness", &globals::hacks::aimbot::smoothness, 1.0f, 100.0f);

            ImGui::EndChild();
        }
    }
}

#endif // CENTURY_AIMBOT_HPP
