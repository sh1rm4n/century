//
// Created by sh1rm4n on 6/5/26.
//

#ifndef CENTURY_NETWORK_HPP
#define CENTURY_NETWORK_HPP

#include "../globals.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

#include "../interfaces.hpp"

namespace menu::pages::miscellaneous {
    void Render() noexcept {
        static std::string reason = "imagine gmod cheating on arch :skull:";

        ImGui::InputText("Disconnect reason", &reason);
        if (ImGui::Button("Custom disconnect")) {
            const auto netchan = interfaces->engineClient->GetNetChannelInfo();

            if (netchan)
                netchan->Shutdown(reason.c_str());
            else
                ImGui::InsertNotification({ImGuiToastType::Error, "Please join any server."});
        }

        ImGui::Checkbox("Engine prediction", &globals::Core::enginePrediction);

        if (ImGui::BeginChild("##FakeLag", ImVec2(240.f, 0.f), ImGuiChildFlags_Borders)) {
            static const char* modes[] = {"Static\0", "Adaptive\0"};

            ImGui::SeparatorText("Fake Lag");

            ImGui::Checkbox("Enabled", &globals::Hacks::FakeLag::enabled);
            ImGui::Combo("Lag mode", &globals::Hacks::FakeLag::mode, modes, IM_ARRAYSIZE(modes));
            ImGui::SliderInt("Lag ticks", &globals::Hacks::FakeLag::ticks, 1, 14);

            ImGui::EndChild();
        }

        ImGui::SameLine();

        if (ImGui::BeginChild("##Killfeed", ImVec2(0.f, 0.f), ImGuiChildFlags_Borders)) {
            ImGui::SeparatorText("Killfeed");

            ImGui::Checkbox("Enabled", &globals::Hacks::Killfeed::enabled);

            if (ImGui::BeginListBox("Killfeed words")) {
                for (const auto& word : globals::Hacks::Killfeed::words) {
                    if (ImGui::Selectable(word.c_str())) {
                        globals::Hacks::Killfeed::words.erase(std::remove(globals::Hacks::Killfeed::words.begin(), globals::Hacks::Killfeed::words.end(), word), globals::Hacks::Killfeed::words.end());
                    }
                }

                ImGui::EndListBox();
            }

            static std::string wordToAdd;

            ImGui::InputText("Word to add", &wordToAdd);
            if (ImGui::Button("Add"))
                globals::Hacks::Killfeed::words.push_back(wordToAdd);

            ImGui::EndChild();
        }
    }
}

#endif //CENTURY_NETWORK_HPP
