//
// Created by sh1rm4n on 7/23/26.
//

#ifndef CENTURY_ENDSCENE_HPP
#define CENTURY_ENDSCENE_HPP

#pragma once

#include <polyhook2/Detour/x64Detour.hpp>

#include "../3rd/imgui/imgui_impl_dx9.h"
#include "../3rd/imgui/imgui_impl_win32.h"

#include "../3rd/fonts/robotoFont.hpp"
#include "../3rd/fonts/firaCodeFont.hpp"
#include "../3rd/fonts/fa-solid-900.h"

#include "../core/menu.hpp"

#include <imgui_stdlib.h>

inline std::uint64_t ogEndScene = 0;

long dtEndScene(IDirect3DDevice9* device) {
	static auto ogFunction = PLH::FnCast(ogEndScene, dtEndScene);

    static bool init = false;

    if (!init) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        menu::Setup();

        ImGui_ImplWin32_Init(FindWindowA(0, "Garry's Mod (64-bit)"));
        ImGui_ImplDX9_Init(device);

        auto& temp = ImGui::GetIO();

        temp.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        temp.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        temp.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        temp.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        temp.IniFilename = nullptr;
        temp.LogFilename = nullptr;

        float baseFontSize = 16.0f;
        float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

        temp.Fonts->AddFontFromMemoryTTF(robotoFont, sizeof(robotoFont), baseFontSize, nullptr, temp.Fonts->GetGlyphRangesCyrillic());

        menu::pages::lua::editorFont = temp.Fonts->AddFontFromMemoryTTF(firaCodeFont, sizeof(firaCodeFont), baseFontSize, nullptr, temp.Fonts->GetGlyphRangesCyrillic());

        static constexpr ImWchar iconsRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphMinAdvanceX = iconFontSize;
        temp.Fonts->AddFontFromMemoryTTF(fa_solid_900, sizeof(fa_solid_900), iconFontSize, &iconsConfig, iconsRanges);

        init = true;
    }

    static ImGuiIO& io = ImGui::GetIO();

    device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
    device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    if (globals::Menu::active)
        menu::Render();

    ImGui::EndFrame();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 1.00f));

    ImGui::RenderNotifications();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(1);

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

	return ogFunction(device);
}

#endif //CENTURY_ENDSCENE_HPP
