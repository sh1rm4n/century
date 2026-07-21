//
// Created by sh1rm4n on 5/24/26.
//

#ifndef CENTURY_MENU_HPP
#define CENTURY_MENU_HPP

#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include "../3rd/imgui/imanim/im_anim.h"
#include "../3rd/imgui/imgui_notify/ImGuiNotify.hpp"

#include "pages/visuals.hpp"
#include "pages/accuracy.hpp"
#include "pages/miscellaneous.hpp"
#include "pages/lua.hpp"
#include "pages/friends.hpp"

enum MenuTab : int {
    Console,
    Visuals,
    Accuracy,
    Movement,
    Miscellaneous,
    Lua,
    Friends,
    Config
};

namespace ImGui {
    inline bool AnimatedButton(const char *label) {
        ImGui::PushID(label);
        ImGuiID id = ImGui::GetID("btn");
        float dt = ImGui::GetIO().DeltaTime;

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
}

namespace menu {
    inline MenuTab currentTab = MenuTab::Console;

    inline void Setup() {
        ImGuiStyle &style = ImGui::GetStyle();

        auto& colors = style.Colors;

        style.WindowRounding = 5.f;
        style.FrameRounding = 4.f;
        style.ChildRounding = 4.f;
        style.PopupRounding = 4.f;
        style.ScrollbarRounding = 4.f;
        style.GrabRounding = 4.f;
        style.TabRounding = 4.f;

        style.WindowPadding = ImVec2(10.0f, 10.0f);
        style.WindowMinSize = ImVec2(32.0f, 32.0f);
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_None;

        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

        style.WindowBorderSize = 2.f;
        style.FrameBorderSize = 0.f;
        style.PopupBorderSize = 0.f;

        style.TabBarOverlineSize = 2.f;

        colors[ImGuiCol_Text] = ImVec4(0.85f, 0.90f, 0.85f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.55f, 0.50f, 1.00f);

        // Backgrounds
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.09f, 0.06f, 1.00f); // Deep pine
        colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.11f, 0.08f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.10f, 0.07f, 0.96f);

        // Borders
        colors[ImGuiCol_Border] = ImVec4(0.18f, 0.28f, 0.18f, 0.80f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Frames (Inputs, Checkboxes, etc.)
        colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.18f, 0.12f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.30f, 0.18f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.42f, 0.24f, 1.00f);

        // Title Bars
        colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.14f, 0.09f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.26f, 0.14f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05f, 0.08f, 0.05f, 1.00f);

        // Menus
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.14f, 0.09f, 1.00f);

        // Scrollbars
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.08f, 0.05f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.18f, 0.28f, 0.18f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.38f, 0.25f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.32f, 0.48f, 0.32f, 1.00f);

        // Interactables
        colors[ImGuiCol_CheckMark] = ImVec4(0.45f, 0.75f, 0.45f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.55f, 0.35f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.45f, 0.70f, 0.45f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.18f, 0.35f, 0.18f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.45f, 0.25f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.32f, 0.55f, 0.32f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.18f, 0.35f, 0.18f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.45f, 0.25f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.32f, 0.55f, 0.32f, 1.00f);

        // Separators and Resizing
        colors[ImGuiCol_Separator] = ImVec4(0.18f, 0.28f, 0.18f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.25f, 0.45f, 0.25f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.32f, 0.55f, 0.32f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.18f, 0.35f, 0.18f, 0.80f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 0.45f, 0.25f, 1.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.32f, 0.55f, 0.32f, 1.00f);

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.22f, 0.12f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.45f, 0.25f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.38f, 0.20f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.15f, 0.08f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.12f, 0.22f, 0.12f, 1.00f);

        // Plots
        colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.70f, 0.40f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.50f, 0.85f, 0.50f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.70f, 0.40f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.50f, 0.85f, 0.50f, 1.00f);

        // Tables
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.12f, 0.22f, 0.12f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.20f, 0.35f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.15f, 0.25f, 0.15f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.08f, 0.14f, 0.08f, 0.50f);

        // Misc
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 0.55f, 0.25f, 0.50f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.60f, 0.90f, 0.60f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.40f, 0.80f, 0.40f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.85f, 0.90f, 0.85f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.10f, 0.15f, 0.10f, 0.50f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.05f, 0.08f, 0.05f, 0.60f);
    }

    inline void Render() noexcept {
        ImGui::ShowDemoWindow();

        ImGui::SetNextWindowSize(ImVec2(600, 350));

        /*auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm* local = std::localtime(&t);*/

        if (ImGui::Begin("century", nullptr, ImGuiWindowFlags_NoResize)) {
            if (ImGui::BeginChild("##Topbar", ImVec2(0.f, 25.f))) {
                if (ImGui::BeginTabBar("##Pages", ImGuiTabBarFlags_DrawSelectedOverline)) {
                    if (ImGui::BeginTabItem("Console")) {
                        currentTab = Console;

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Accuracy")) {
                        currentTab = Accuracy;

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Movement")) {
                        currentTab = Movement;

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Visuals")) {
                        currentTab = Visuals;

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Miscellaneous")) {
                        currentTab = Miscellaneous;

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Lua")) {
                        currentTab = Lua;

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Friends")) {
                        currentTab = Friends;

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Config")) {
                        currentTab = Config;

                        ImGui::EndTabItem();
                    }

                    ImGui::EndTabBar();
                }

                ImGui::EndChild();
            }

            if (ImGui::BeginChild("##Container", ImVec2(0.f, 0))) {
                switch (currentTab) {
                    case Console:
                        break;
                    case Visuals:
                        pages::visuals::Render();
                        break;
                    case Accuracy:
                        pages::accuracy::Render();
                        break;
                    case Movement:
                        break;
                    case Miscellaneous:
                        pages::miscellaneous::Render();
                        break;
                    case Lua:
                        pages::lua::Render();
                        break;
                    case Friends:
                        pages::friends::Render();
                        break;
                    case Config:
                        break;
                }

                ImGui::EndChild();
            }

            ImGui::End();
        }
    }
}

#endif //CENTURY_MENU_HPP
