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
        ImGui::ToggleAnimated("Enable chams?", &globals::Hacks::Chams::enabled);
        ImGui::InputText("Chams material", &globals::Hacks::Chams::material);
        if (ImGui::BeginCombo("Chams flags", "")) {
            ImGui::CheckboxFlags("Selfillum", &globals::Hacks::Chams::flags, CHAMS_SELFILLUM);
            ImGui::CheckboxFlags("Additive", &globals::Hacks::Chams::flags, CHAMS_ADDITIVE);
            ImGui::CheckboxFlags("No fog", &globals::Hacks::Chams::flags, CHAMS_NOFOG);
            ImGui::CheckboxFlags("Half lambert", &globals::Hacks::Chams::flags, CHAMS_HALFLAMBERT);
            ImGui::CheckboxFlags("Multipass", &globals::Hacks::Chams::flags, CHAMS_MULTIPASS);
            ImGui::CheckboxFlags("Envmap", &globals::Hacks::Chams::flags, CHAMS_ENVMAP);
            ImGui::CheckboxFlags("Flat", &globals::Hacks::Chams::flags, CHAMS_FLAT);
            ImGui::CheckboxFlags("Wireframe", &globals::Hacks::Chams::flags, CHAMS_WIREFRAME);
            ImGui::CheckboxFlags("No cull", &globals::Hacks::Chams::flags, CHAMS_NOCULL);
            ImGui::EndCombo();
        }
        ImGui::ColorPicker3("Visible colors", globals::Hacks::Chams::visibleColors);
        ImGui::ColorPicker3("Hidden colors", globals::Hacks::Chams::hiddenColors);
    }
}

#endif //CENTURY_CHAMS_HPP
