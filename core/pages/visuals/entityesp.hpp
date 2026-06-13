//
// Created by sh1rm4n on 5/30/26.
//

#ifndef CENTURY_ENTITYESP_HPP
#define CENTURY_ENTITYESP_HPP

#include "../../globals.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>
#include "../../../3rd/imgui/imgui_toggle/imgui_toggle.h"

namespace menu::pages::entityesp {
    void Render() noexcept {
        static std::string searchFor;
        static bool showLuaBasedOnly = false;

        ImGui::ToggleAnimated("Enable Entity ESP?", &globals::hacks::entityesp::enabled);
        ImGui::InputText("Search for ents", &searchFor);
        if (ImGui::Button("Select all ents")) {
            for (const auto& ent : globals::hacks::entityesp::entsToDraw) {
                globals::hacks::entityesp::allowedEntsToDraw.insert(ent.name);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Select all lua ents")) {
            for (const auto& ent : globals::hacks::entityesp::entsToDraw) {
                if (ent.luaBased != true)
                    continue;

                globals::hacks::entityesp::allowedEntsToDraw.insert(ent.name);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Remove all ents")) {
            globals::hacks::entityesp::allowedEntsToDraw.clear();
        }
        ImGui::ToggleAnimated("Lua ents only", &showLuaBasedOnly);
        if (ImGui::BeginListBox("Ents")) {
            for (const auto& ent : globals::hacks::entityesp::entsToDraw) {
                // Check if this specific entity string is in our allowed set
                bool is_allowed = globals::hacks::entityesp::allowedEntsToDraw.contains(ent.name);

                if (showLuaBasedOnly && ent.luaBased != showLuaBasedOnly)
                    continue;

                if (!ent.name.contains(searchFor))
                    continue;

                // Pass the state to the selectable widget
                if (ImGui::Selectable(ent.name.c_str(), is_allowed)) {
                    if (is_allowed) {
                        globals::hacks::entityesp::allowedEntsToDraw.erase(ent.name); // Remove if clicked while active
                    } else {
                        globals::hacks::entityesp::allowedEntsToDraw.insert(ent.name); // Add if clicked while inactive
                    }
                }
            }

            ImGui::EndListBox();
        }

        if (ImGui::BeginListBox("Selected ents")) {
            std::string entToRemove = ""; // Temporary storage for deferred erasure

            for (const auto& ent : globals::hacks::entityesp::allowedEntsToDraw) {
                if (!ent.contains(searchFor))
                    continue;

                // Pass 'true' as the second argument so the item looks highlighted/selected
                if (ImGui::Selectable(ent.c_str(), true)) {
                    entToRemove = ent; // Mark this element for deletion
                }
            }

            // Safely erase outside the loop iteration context
            if (!entToRemove.empty()) {
                globals::hacks::entityesp::allowedEntsToDraw.erase(entToRemove);
            }

            ImGui::EndListBox();
        }
    }
}

#endif //CENTURY_ENTITYESP_HPP
