//
// Created by sh1rm4n on 7/7/26.
//

#ifndef CENTURY_VISUALS_HPP
#define CENTURY_VISUALS_HPP

#include "../../3rd/imgui/imgui_toggle/imgui_toggle.h"

namespace menu::pages::visuals {
    void Render() noexcept {
        ImGui::ToggleAnimated("Enable Player ESP?", &globals::Hacks::PlayerESP::enabled);
        ImGui::ToggleAnimated("Bones", &globals::Hacks::PlayerESP::bones);
        //ImGui::Checkbox("Square ESP", &globals::hacks::playeresp::square);
        //ImGui::Checkbox("3D Box ESP", &globals::hacks::playeresp::box);
    }
}

#endif //CENTURY_VISUALS_HPP
