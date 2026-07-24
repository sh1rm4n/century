//
// Created by sh1rm4n on 6/9/26.
//

#ifndef CENTURY_AIMBOT_HPP
#define CENTURY_AIMBOT_HPP

#include "../globals.hpp"

#include "../../3rd/imgui/imgui_toggle/imgui_toggle.h"

namespace menu::pages::accuracy {
    void Render() noexcept {
        ImGui::BeginChild("##Aimbot", ImVec2(240, 0), ImGuiChildFlags_Borders);
        ImGui::SeparatorText("Aimbot");
        ImGui::ToggleAnimated("Enabled", &globals::Hacks::Aimbot::enabled);
        ImGui::ToggleAnimated("AA Bruteforce", &globals::Hacks::Aimbot::bruteforce);
        ImGui::ToggleAnimated("Silent aim", &globals::Hacks::Aimbot::silentAim);
        ImGui::ToggleAnimated("pSilent aim", &globals::Hacks::Aimbot::perfectSilent);
        ImGui::ToggleAnimated("Team checking", &globals::Hacks::Aimbot::teamCheck);
        ImGui::SliderFloat("FOV", &globals::Hacks::Aimbot::fov, 1.0f, 180.0f);
        ImGui::SliderFloat("Smooth", &globals::Hacks::Aimbot::smoothness, 1.0f, 100.0f);
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("##RightColumn", ImVec2(0, 0), 0);
        {
            const float rowHeight = ImGui::GetContentRegionAvail().y * 0.5f;

            ImGui::BeginChild("##ForwardTracking", ImVec2(0, rowHeight), ImGuiChildFlags_Borders);
            ImGui::SeparatorText("Forward track");
            ImGui::ToggleAnimated("Enabled", &globals::Hacks::ForwardTracking::enabled);
            ImGui::SliderFloat("Ghetto pred", &globals::Hacks::ForwardTracking::ghettoPredictionAmount, 1.0f, 5.0f);
            ImGui::EndChild();

            ImGui::BeginChild("##BackTracking", ImVec2(0, 0), ImGuiChildFlags_Borders);
            ImGui::SeparatorText("Back track");
            ImGui::ToggleAnimated("Enabled", &globals::Hacks::BackTracking::enabled);

            ImGui::SliderInt("Ticks", &globals::Hacks::BackTracking::ticks, 1, 10);

            ImGui::EndChild();
        }
        ImGui::EndChild();
    }
}

#endif // CENTURY_AIMBOT_HPP
