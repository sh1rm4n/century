//
// Created by sh1rm4n on 6/3/26.
//

#ifndef CENTURY_OTHER_HPP
#define CENTURY_OTHER_HPP

#include "../../../core/globals.hpp"

#include <imgui.h>

namespace menu::pages::visuals_other {
    void Render() noexcept {
        ImGui::SliderFloat("FOV", &globals::hacks::visuals_other::fov, 30.0f, 180.0f);
        ImGui::SliderFloat("Viewmodel FOV", &globals::hacks::visuals_other::viewmodelFov, 30.0f, 180.0f);
    }
}

#endif //CENTURY_OTHER_HPP
