//
// Created by sh1rm4n on 7/7/26.
//

#ifndef CENTURY_VISUALS_HPP
#define CENTURY_VISUALS_HPP

#include "../../3rd/imgui/imgui_toggle/imgui_toggle.h"

#include <imgui_stdlib.h>

namespace menu::pages::visuals {
    void Render() noexcept {
        ImGui::ToggleAnimated("Enable Player ESP?", &globals::Hacks::PlayerESP::enabled);
        ImGui::ToggleAnimated("Bones", &globals::Hacks::PlayerESP::bones);
        //ImGui::Checkbox("Square ESP", &globals::hacks::playeresp::square);
        //ImGui::Checkbox("3D Box ESP", &globals::hacks::playeresp::box);

        ImGui::BeginChild("##Chams", ImVec2(0, 0), ImGuiChildFlags_Borders);
        {
            std::string previewValue = "";

            ImGui::ToggleAnimated("Enable chams?", &globals::Hacks::Chams::enabled);
            ImGui::InputText("Chams material", &globals::Hacks::Chams::material);
            if (ImGui::BeginCombo("Chams flags", "")) {
                if (ImGui::CheckboxFlags("Selfillum", &globals::Hacks::Chams::flags, CHAMS_SELFILLUM))
                    previewValue.append("Selfillum ");

                if (ImGui::CheckboxFlags("Additive", &globals::Hacks::Chams::flags, CHAMS_ADDITIVE))
                    previewValue.append("Additive ");

                if (ImGui::CheckboxFlags("No fog", &globals::Hacks::Chams::flags, CHAMS_NOFOG))
                    previewValue.append("No fog ");

                if (ImGui::CheckboxFlags("Half lambert", &globals::Hacks::Chams::flags, CHAMS_HALFLAMBERT))
                    previewValue.append("Half lambert ");

                if (ImGui::CheckboxFlags("Multipass", &globals::Hacks::Chams::flags, CHAMS_MULTIPASS))
                    previewValue.append("Multipass ");

                if (ImGui::CheckboxFlags("Envmap", &globals::Hacks::Chams::flags, CHAMS_ENVMAP))
                    previewValue.append("Envmap ");

                if (ImGui::CheckboxFlags("Flat", &globals::Hacks::Chams::flags, CHAMS_FLAT))
                    previewValue.append("Flat ");

                if (ImGui::CheckboxFlags("Wireframe", &globals::Hacks::Chams::flags, CHAMS_WIREFRAME))
                    previewValue.append("Wireframe ");

                if (ImGui::CheckboxFlags("No cull", &globals::Hacks::Chams::flags, CHAMS_NOCULL))
                    previewValue.append("No cull ");

                ImGui::EndCombo();
            }
            ImGui::ColorEdit3("Visible colors", globals::Hacks::Chams::visibleColors, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit3("Hidden colors", globals::Hacks::Chams::hiddenColors, ImGuiColorEditFlags_NoInputs);
        }
        ImGui::EndChild();

        ImGui::BeginChild("##EntityESP", ImVec2(0, 0), ImGuiChildFlags_Borders);
        {
            static std::string searchFor;
            static bool showLuaBasedOnly = false;

            ImGui::ToggleAnimated("Enable Entity ESP?", &globals::Hacks::EntityESP::enabled);
            ImGui::InputText("Search for ents", &searchFor);
            if (ImGui::Button("Select all ents")) {
                for (const auto& ent : globals::Hacks::EntityESP::entsToDraw) {
                    globals::Hacks::EntityESP::allowedEntsToDraw.insert(ent.name);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Select all lua ents")) {
                for (const auto& ent : globals::Hacks::EntityESP::entsToDraw) {
                    if (ent.luaBased != true)
                        continue;

                    globals::Hacks::EntityESP::allowedEntsToDraw.insert(ent.name);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Remove all ents")) {
                globals::Hacks::EntityESP::allowedEntsToDraw.clear();
            }
            ImGui::ToggleAnimated("Lua ents only", &showLuaBasedOnly);
            if (ImGui::BeginListBox("Ents")) {
                for (const auto& ent : globals::Hacks::EntityESP::entsToDraw) {
                    // Check if this specific entity string is in our allowed set
                    bool is_allowed = globals::Hacks::EntityESP::allowedEntsToDraw.contains(ent.name);

                    if (showLuaBasedOnly && ent.luaBased != showLuaBasedOnly)
                        continue;

                    if (!ent.name.contains(searchFor))
                        continue;

                    // Pass the state to the selectable widget
                    if (ImGui::Selectable(ent.name.c_str(), is_allowed)) {
                        if (is_allowed) {
                            globals::Hacks::EntityESP::allowedEntsToDraw.erase(ent.name); // Remove if clicked while active
                        } else {
                            globals::Hacks::EntityESP::allowedEntsToDraw.insert(ent.name); // Add if clicked while inactive
                        }
                    }
                }

                ImGui::EndListBox();
            }

            if (ImGui::BeginListBox("Selected ents")) {
                std::string entToRemove = ""; // Temporary storage for deferred erasure

                for (const auto& ent : globals::Hacks::EntityESP::allowedEntsToDraw) {
                    if (!ent.contains(searchFor))
                        continue;

                    // Pass 'true' as the second argument so the item looks highlighted/selected
                    if (ImGui::Selectable(ent.c_str(), true)) {
                        entToRemove = ent; // Mark this element for deletion
                    }
                }

                // Safely erase outside the loop iteration context
                if (!entToRemove.empty()) {
                    globals::Hacks::EntityESP::allowedEntsToDraw.erase(entToRemove);
                }

                ImGui::EndListBox();
            }
        }
        ImGui::EndChild();
    }
}

#endif //CENTURY_VISUALS_HPP
