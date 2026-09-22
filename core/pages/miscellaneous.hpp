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
        if (ImGui::BeginChild("##MiscColumn1", ImVec2(275.f, 0.f))) {
            if (ImGui::BeginChild("##FakeLag", ImVec2(0.f, 175.f), ImGuiChildFlags_Borders)) {
                static const char* modes[] = {"Static", "Adaptive"};
                ImGui::SeparatorText("Fake Lag");
                ImGui::Checkbox("Enabled", &globals::Hacks::FakeLag::enabled);
                ImGui::Combo("Lag mode", &globals::Hacks::FakeLag::mode, modes, IM_ARRAYSIZE(modes));
                ImGui::SliderInt("Lag ticks", &globals::Hacks::FakeLag::ticks, 1, 14);
                ImGui::EndChild();
            }

            if (ImGui::BeginChild("##Slowmotion", ImVec2(0.f, 175.f), ImGuiChildFlags_Borders)) {
                ImGui::SeparatorText("Slowmotion");
                ImGui::Checkbox("Enabled", &globals::Hacks::Slowmotion::enabled);
                ImGui::SliderInt("Slow ticks", &globals::Hacks::Slowmotion::ticks, 1, 15);
                ImGui::EndChild();
            }

            ImGui::EndChild();
        }

        ImGui::SameLine();

        if (ImGui::BeginChild("##MiscColumn2", ImVec2(0.f, 0.f))) {
            if (ImGui::BeginChild("##Killfeed", ImVec2(0.f, 0.f), ImGuiChildFlags_Borders)) {
                ImGui::SeparatorText("Killfeed");
                ImGui::Checkbox("Enabled", &globals::Hacks::Killfeed::enabled);

                if (ImGui::BeginListBox("Killfeed words")) {
                    for (std::size_t i = 0; i < globals::Hacks::Killfeed::words.size(); ++i) {
                        if (ImGui::Selectable(globals::Hacks::Killfeed::words[i].c_str())) {
                            globals::Hacks::Killfeed::words.erase(globals::Hacks::Killfeed::words.begin() + i);
                            break;
                        }
                    }
                    ImGui::EndListBox();
                }

                static std::string wordToAdd;
                ImGui::InputText("Word to add", &wordToAdd);

                if (ImGui::Button("Add") && !wordToAdd.empty()) {
                    globals::Hacks::Killfeed::words.push_back(wordToAdd);
                    wordToAdd.clear();
                }

                ImGui::EndChild();
            }

            if (ImGui::Button("top secret")) {
                const auto netChan = interfaces->engineClient->GetNetChannelInfo();

                if (netChan) {
                    netChan->Shutdown("FUCK NIGGERS");
                }
            }

            ImGui::EndChild();
        }
    }
}

#endif //CENTURY_NETWORK_HPP
