//
// Created by sh1rm4n on 5/31/26.
//

#ifndef CENTURY_PLAYERESP_HPP
#define CENTURY_PLAYERESP_HPP

#include "../../globals.hpp"

#include <imgui.h>
#include "../../../3rd/imgui/imgui_toggle/imgui_toggle.h"

namespace menu::pages::playeresp {
    void Render() noexcept {
        ImGui::ToggleAnimated("Enable Player ESP?", &globals::hacks::playeresp::enabled);
        ImGui::ToggleAnimated("Bones", &globals::hacks::playeresp::bones);
        //ImGui::Checkbox("Square ESP", &globals::hacks::playeresp::square);
        //ImGui::Checkbox("3D Box ESP", &globals::hacks::playeresp::box);
    }
}

#endif //CENTURY_PLAYERESP_HPP
