//
// Created by sh1rm4n on 5/24/26.
//

#ifndef CENTURY_MENU_HPP
#define CENTURY_MENU_HPP

#include <imgui.h>
#include <imgui_internal.h>

#include "globals.hpp"

#include "../3rd/imgui/imanim/im_anim.h"

#include "../3rd/imgui/imgui_notify/ImGuiNotify.hpp"

#include "pages/visuals/chams.hpp"
#include "pages/visuals/entityesp.hpp"
#include "pages/visuals/other.hpp"
#include "pages/visuals/playeresp.hpp"

#include "pages/aiming/aimbot.hpp"

#include "pages/lua/executor.hpp"

#include "pages/misc/network.hpp"

enum MenuTab : int {
    Console,
    Visuals,
    Aiming,
    Lua,
    Miscellaneous
};

enum MenuPages : int {
    Visuals_Chams,
    Visuals_EntityESP,
    Visuals_PlayerESP,
    Visuals_Other,
    Aiming_Aimbot,
    Lua_Executor,
    Misc_Network
};

namespace menu {
    inline MenuTab currentTab = MenuTab::Console;
    inline MenuPages currentPage = MenuPages::Visuals_Chams;

    inline bool AnimatedButton(const char *label) {
        ImGui::PushID(label);
        ImGuiID id = ImGui::GetID("btn");
        float dt = ImGui::GetIO().DeltaTime;

        // Need to check hover before the button for this frame
        ImVec2 size = ImGui::CalcTextSize(label);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        bool hovered = ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x + size.x + 20, pos.y + size.y + 10));

        ImVec4 color = iam_tween_color(id, ImHashStr("color"),
                                       hovered ? ImVec4(0.3f, 0.7f, 1.0f, 1.0f) : ImVec4(0.2f, 0.2f, 0.2f, 1.0f),
                                       0.2f, iam_ease_preset(iam_ease_out_sine),
                                       iam_policy_crossfade, iam_col_oklab, dt);

        ImGui::PushStyleColor(ImGuiCol_Button, color);
        if (ImGui::Button(label)) {
            return true;
        }
        ImGui::PopStyleColor();
        ImGui::PopID();
        return false;
    }

    inline void AnimatedTabButton(const char *label, MenuTab page) {
        ImGui::PushID(label);
        ImGuiID id = ImGui::GetID("btn");
        float dt = ImGui::GetIO().DeltaTime;

        // Need to check hover before the button for this frame
        ImVec2 size = ImGui::CalcTextSize(label);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        bool hovered = ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x + size.x + 20, pos.y + size.y + 10));

        ImVec4 color = iam_tween_color(id, ImHashStr("color"),
                                       hovered ? ImVec4(0.3f, 0.7f, 1.0f, 1.0f) : ImVec4(0.2f, 0.2f, 0.2f, 1.0f),
                                       0.2f, iam_ease_preset(iam_ease_out_sine),
                                       iam_policy_crossfade, iam_col_oklab, dt);

        ImGui::PushStyleColor(ImGuiCol_Button, color);
        if (ImGui::Button(label)) {
            currentTab = page;
        }
        ImGui::PopStyleColor();
        ImGui::PopID();
    }

    inline void Setup() {
        ImGuiStyle &style = ImGui::GetStyle();

        style.WindowRounding = 5.f;
        style.FrameRounding = 4.f;
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

        style.WindowPadding = ImVec2(10.0f, 10.0f);
        style.WindowMinSize = ImVec2(32.0f, 32.0f);
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_None;

        style.WindowBorderSize = 0.f;
        style.FrameBorderSize = 0.f;
        style.PopupBorderSize = 0.f;

        style.Colors[ImGuiCol_TitleBgActive] = {0.541, 0.0, 1.0, 0.591};
    }

    inline void Render() noexcept {
        //ImGui::ShowDemoWindow();

        ImGui::SetNextWindowSize(ImVec2(600, 350));
        ImGui::Begin("century", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        if (ImGui::BeginChild("##Topbar", ImVec2(0.f, 20.f))) {
            ImGui::TextColored(ImColor(85, 0, 255), "century");
            ImGui::SameLine();
            AnimatedTabButton("Console", Console);
            ImGui::SameLine();
            AnimatedTabButton("Visuals", Visuals);
            ImGui::SameLine();
            AnimatedTabButton("Aiming", Aiming);
            ImGui::SameLine();
            AnimatedTabButton("Lua", Lua);
            ImGui::SameLine();
            AnimatedTabButton("Miscellaneous", Miscellaneous);

            ImGui::EndChild();
        }

        if (ImGui::BeginChild("##Sidebar", ImVec2(70.0f, 0.0f))) {
            switch (currentTab) {
                default: break;

                case MenuTab::Console: {
                    ImGui::Text("?");
                }
                break;

                case MenuTab::Visuals: {
                    if (AnimatedButton("Chams")) {
                        currentPage = MenuPages::Visuals_Chams;
                    } else if (AnimatedButton("Entity ESP")) {
                        currentPage = MenuPages::Visuals_EntityESP;
                    } else if (AnimatedButton("Player ESP")) {
                        currentPage = MenuPages::Visuals_PlayerESP;
                    } else if (AnimatedButton("Other")) {
                        currentPage = MenuPages::Visuals_Other;
                    }
                } break;

                case MenuTab::Aiming: {
                    if (AnimatedButton("Aimbot")) {
                        currentPage = MenuPages::Aiming_Aimbot;
                    }
                } break;

                case MenuTab::Lua: {
                    if (AnimatedButton("Executor")) {
                        currentPage = MenuPages::Lua_Executor;
                    }
                } break;

                case MenuTab::Miscellaneous: {
                    if (AnimatedButton("Network")) {
                        currentPage = MenuPages::Misc_Network;
                    }
                } break;
            }
            ImGui::EndChild();
        }

        ImGui::SameLine();

        if (ImGui::BeginChild("##Container", ImVec2(0.0f, 0.0f))) {
            switch (currentTab) {
                default: break;

                case MenuTab::Console: {
                    ImGui::Text("Console view content goes here");
                }
                break;

                case MenuTab::Visuals: {
                    if (currentPage == MenuPages::Visuals_Chams) {
                        menu::pages::chams::Render();
                    } else if (currentPage == MenuPages::Visuals_EntityESP) {
                        menu::pages::entityesp::Render();
                    } else if (currentPage == MenuPages::Visuals_PlayerESP) {
                        menu::pages::playeresp::Render();
                    } else if (currentPage == MenuPages::Visuals_Other) {
                        menu::pages::visuals_other::Render();
                    }
                }
                break;

                case MenuTab::Aiming: {
                    if (currentPage == MenuPages::Aiming_Aimbot) {
                        menu::pages::aimbot::Render();
                    }
                }
                break;

                case MenuTab::Lua: {
                    if (currentPage == MenuPages::Lua_Executor) {
                        menu::pages::executor::Render();
                    }
                }

                case MenuTab::Miscellaneous: {
                    if (currentPage == MenuPages::Misc_Network) {
                        menu::pages::network::Render();
                    }
                }
                break;
            }
            ImGui::EndChild();
        }

        ImGui::End();
    }
}

#endif //CENTURY_MENU_HPP
