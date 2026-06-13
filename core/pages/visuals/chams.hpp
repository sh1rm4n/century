//
// Created by sh1rm4n on 5/27/26.
//

#ifndef CENTURY_CHAMS_HPP
#define CENTURY_CHAMS_HPP

#include "../../globals.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include "../../../3rd/imgui/imgui_toggle/imgui_toggle.h"

namespace menu::pages::chams {
    void Render() noexcept {
        ImGui::ToggleAnimated("Enable chams?", &globals::hacks::chams::enabled);
        ImGui::InputText("Chams material", &globals::hacks::chams::material);
        if (ImGui::BeginCombo("Chams flags", "")) {
            ImGui::CheckboxFlags("Selfillum", &globals::hacks::chams::flags, CHAMS_SELFILLUM);
            ImGui::CheckboxFlags("Additive", &globals::hacks::chams::flags, CHAMS_ADDITIVE);
            ImGui::CheckboxFlags("No fog", &globals::hacks::chams::flags, CHAMS_NOFOG);
            ImGui::CheckboxFlags("Half lambert", &globals::hacks::chams::flags, CHAMS_HALFLAMBERT);
            ImGui::CheckboxFlags("Multipass", &globals::hacks::chams::flags, CHAMS_MULTIPASS);
            ImGui::CheckboxFlags("Envmap", &globals::hacks::chams::flags, CHAMS_ENVMAP);
            ImGui::CheckboxFlags("Flat", &globals::hacks::chams::flags, CHAMS_FLAT);
            ImGui::CheckboxFlags("Wireframe", &globals::hacks::chams::flags, CHAMS_WIREFRAME);
            ImGui::CheckboxFlags("No cull", &globals::hacks::chams::flags, CHAMS_NOCULL);
            ImGui::EndCombo();
        }
        ImGui::ColorPicker3("Visible colors", globals::hacks::chams::visibleColors);
        ImGui::ColorPicker3("Hidden colors", globals::hacks::chams::hiddenColors);
    }
}

#endif //CENTURY_CHAMS_HPP
