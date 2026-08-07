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
#include "../3rd/fonts/lilexFont.hpp"
#include "../3rd/fonts/fa-solid-900.h"

#include "../core/menu.hpp"

#include "../sourcesdk/entities/C_CollisionProperty.hpp"
#include "../sourcesdk/entities/C_BaseCombatWeapon.hpp"
#include "../sourcesdk/entities/IClientRenderable.hpp"

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

        globals::Menu::secondaryFont = temp.Fonts->AddFontFromMemoryTTF(lilexFont, sizeof(lilexFont), baseFontSize, nullptr, temp.Fonts->GetGlyphRangesCyrillic());

        static constexpr ImWchar iconsRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphMinAdvanceX = iconFontSize;
        temp.Fonts->AddFontFromMemoryTTF(fa_solid_900, sizeof(fa_solid_900), iconFontSize, &iconsConfig, iconsRanges);

        init = true;
    }

    static ImGuiIO& io = ImGui::GetIO();

    int w, h;
    w = io.DisplaySize.x;
    h = io.DisplaySize.y;

    device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
    device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    iam_update_begin_frame();
    iam_clip_update(io.DeltaTime);

    if (globals::Menu::active)
        menu::Render();

    if (interfaces->engineClient->IsInGame()) {
        ImGui::PushFont(globals::Menu::secondaryFont);

        ImGui::Begin("##Background", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
        {
            const auto maxEntities = interfaces->entityList->GetMaxEntities();
            const auto drawList = ImGui::GetBackgroundDrawList();

            const auto font = ImGui::GetFont();

            if (globals::Hacks::EntityESP::enabled) {
                globals::Hacks::EntityESP::entsToDraw.clear();
                globals::Hacks::EntityESP::entsToDraw.reserve(maxEntities);

                for (auto i = 0; i < maxEntities; i++) {
                    const auto entity = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

                    if (!entity || entity->IsPlayer())
                        continue;

                    const auto clientClass = entity->GetClientClass();
                    if (!clientClass)
                        continue;

                    const auto networkedName = clientClass->GetNetworkedName();
                    const auto luaName = entity->GetLuaScriptName();

                    std::string entIdentifier = std::to_string(i) + ": " + ((luaName && luaName[0] != '\0') ? luaName : networkedName);

                    globals::Hacks::EntityESP::entsToDraw.emplace_back(DrawEntity{entIdentifier, entity->UsesLua()});

                    if (globals::Hacks::EntityESP::allowedEntsToDraw.contains(entIdentifier)) {
                        if (!globals::Core::localPlayer)
                            continue;

                        Vector outOrigin;

                        const auto origin = entity->GetAbsOrigin();

                        if (utils::WorldToScreen(origin, outOrigin)) {
                            const auto mins      = entity->GetCollisionProp()->OBBMins();
                            const auto maxs      = entity->GetCollisionProp()->OBBMaxs();
                            const auto health    = entity->GetHealth();
                            const auto maxHealth = std::max(entity->GetMaxHealth(), 1);
                            const auto model     = entity->GetModel();
                            const char* name     = entity->UsesLua() ? luaName : networkedName;

                            utils::Draw3DBox(drawList, origin, entity->GetAbsAngles(), mins, maxs);

                            // get 2D bounding rect from all 8 projected corners
                            Vector forward, right, up;
                            entity->GetAbsAngles().Vectors(forward, right, up);

                            const std::array<Vector, 8> localCorners = {{
                                {mins.x, mins.y, mins.z},
                                {mins.x, maxs.y, mins.z},
                                {maxs.x, maxs.y, mins.z},
                                {maxs.x, mins.y, mins.z},
                                {mins.x, mins.y, maxs.z},
                                {mins.x, maxs.y, maxs.z},
                                {maxs.x, maxs.y, maxs.z},
                                {maxs.x, mins.y, maxs.z},
                            }};

                            float screenMinX = FLT_MAX, screenMinY = FLT_MAX;
                            float screenMaxX = -FLT_MAX, screenMaxY = -FLT_MAX;
                            bool allVisible = true;

                            for (const auto& lc : localCorners) {
                                Vector worldPos = origin + forward * lc.x + right * lc.y + up * lc.z;
                                Vector screenPos;
                                if (!utils::WorldToScreen(worldPos, screenPos)) { allVisible = false; break; }
                                screenMinX = std::min(screenMinX, screenPos.x);
                                screenMinY = std::min(screenMinY, screenPos.y);
                                screenMaxX = std::max(screenMaxX, screenPos.x);
                                screenMaxY = std::max(screenMaxY, screenPos.y);
                            }

                            if (!allVisible)
                                continue;

                            const float boxW    = screenMaxX - screenMinX;
                            const float centerX = screenMinX + boxW * 0.5f;

                            // --- name (centered, above box) ---
                            const ImVec2 nameSize = ImGui::CalcTextSize(name);
                            drawList->AddText(
                                { centerX - nameSize.x * 0.5f, screenMinY - nameSize.y - 2.f },
                                ImColor(255, 255, 255, 255),
                                name);

                            // --- model (centered, above name) ---
                            if (model) {
                                const ImVec2 modSize = ImGui::CalcTextSize(model->strName);
                                drawList->AddText(
                                    { centerX - modSize.x * 0.5f, screenMinY - nameSize.y - modSize.y - 4.f },
                                    ImColor(180, 180, 180, 255),
                                    model->strName);
                            }

                            // --- health bar (bottom, full width of box) ---
                            {
                                const float percent  = std::clamp(static_cast<float>(health) / maxHealth, 0.f, 1.f);
                                const float barH     = 4.f;
                                const float barOff   = 4.f;

                                const ImVec2 bgMin = { screenMinX,                        screenMaxY + barOff };
                                const ImVec2 bgMax = { screenMaxX,                        screenMaxY + barOff + barH };
                                const ImVec2 fgMin = bgMin;
                                const ImVec2 fgMax = { screenMinX + boxW * percent,       bgMax.y };

                                const int r = static_cast<int>((1.f - percent) * 255.f);
                                const int g = static_cast<int>(percent * 255.f);

                                // shadow/bg
                                drawList->AddRectFilled(bgMin, bgMax, ImColor(0, 0, 0, 180));
                                // fill
                                drawList->AddRectFilled(fgMin, fgMax, ImColor(r, g, 0, 255));
                                // border
                                drawList->AddRect(bgMin, bgMax, ImColor(0, 0, 0, 255), 0.f, 0, 1.f);

                                // health text centered below bar
                                const auto hpStr    = std::format("{}/{}", health, maxHealth);
                                const ImVec2 hpSize = ImGui::CalcTextSize(hpStr.c_str());
                                drawList->AddText(
                                    { centerX - hpSize.x * 0.5f, bgMax.y + 2.f },
                                    ImColor(r, g, 0, 255),
                                    hpStr.c_str());
                            }
                        }
                    }
                }
            }

            if (globals::Hacks::PlayerESP::enabled) {
                for (auto i = 1; i <= interfaces->globalVars->maxClients; i++) {
                    const auto entity = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

                    if (!entity || entity == utils::GetLocalPlayer() || entity->IsDormant() || !entity->IsPlayer() || !entity->IsAlive())
                        continue;

                    player_info_s info;
                    if (!interfaces->engineClient->GetPlayerInfo(i, &info))
                        continue;

                    const auto weapon = entity->GetActiveWeapon();
                    if (!weapon)
                        continue;

                    const auto head = entity->GetEyePosition();
                    const auto bottom = entity->GetAbsOrigin();

                    if (globals::Hacks::PlayerESP::bones) {
                        std::array<Matrix3x4, 256> bones = {};
                        if (!entity->SetupBones(bones.data(), 256, 0x7FF00, interfaces->globalVars->curTime))
                            continue;

                        utils::DrawBones(drawList, bones);
                    }

                    if (globals::Hacks::PlayerESP::renderingType == PlayerESPRenderingType::Square) {
                        Vector bottomOut;
                        Vector topOut;

                        const auto top = head + Vector{0.f,0.f,5.f};

                        const auto healthFrac = std::clamp(
                            static_cast<float>(entity->GetHealth()) / entity->GetMaxHealth(), 0.0f, 1.0f);

                        const auto armorFrac = std::clamp(
                            static_cast<float>(interfaces->playerResource->GetArmor(entity->EntIndex())) / entity->GetMaxArmor(), 0.0f, 1.0f);

                        const auto weaponName = weapon->GetName();

                        const auto color = ImColor{255,255,255};
                        const auto outlineColor = ImColor{10,10,10};
                        const auto healthColor = ImColor{static_cast<int>(255.0f * (1.0f - healthFrac)), static_cast<int>(255 * healthFrac), 0};
                        const auto armorColor = ImColor{3, 107, 252};

                        if (utils::WorldToScreen(bottom, bottomOut) &&
                            utils::WorldToScreen(top, topOut)) {
                            const auto height = bottomOut.y - topOut.y;
                            const auto width = height * 0.25f;

                            const auto left = topOut.x - width;
                            const auto right = topOut.x + width;

                            const float fontSize = ImGui::GetFontSize();

                            const auto heightOffset = 25.0f;

                            const ImVec2 weaponNamePos = {
                                topOut.x - ImGui::CalcTextSize(weaponName).x * 0.5f,
                                topOut.y - heightOffset * 1
                            };

                            const ImVec2 namePos = {
                                topOut.x - ImGui::CalcTextSize(info.name).x * 0.5f,
                                topOut.y - heightOffset * 1.6f
                            };

                            utils::DrawOutlinedText(drawList, font, fontSize, weaponNamePos, outlineColor, ImColor{255,255,255}, weaponName);
                            utils::DrawOutlinedText(drawList, font, fontSize, namePos, outlineColor, ImColor{255,255,255}, info.name);

                            utils::DrawOutlinedText(drawList, font, fontSize, {right + 2.f, topOut.y}, outlineColor, healthColor, fmt::format("Health: {}", entity->GetHealth()));
                            utils::DrawOutlinedText(drawList, font, fontSize, {right + 2.f, topOut.y + 12.f}, outlineColor, armorColor, fmt::format("Armor: {}", interfaces->playerResource->GetArmor(i)));

                            drawList->AddRect(
                                {left, topOut.y},
                                {right, bottomOut.y},
                                color, 2.5, 1.5);

                            drawList->AddRect(
                                {left - 1, topOut.y - 1},
                                {right + 1, bottomOut.y + 1},
                                outlineColor, 2.5, 1);

                            // health bar

                            drawList->AddRectFilled(
                                {left - 1, bottomOut.y + 3},
                                {right + 1, bottomOut.y + 10 + 3},
                                ImColor{50,50,50, 200}, 2.5);

                            drawList->AddRectFilled(
                                {left, bottomOut.y + 3},
                                {std::lerp(left, right, healthFrac), bottomOut.y + 10 + 3},
                                healthColor, 2.5);

                            drawList->AddRect(
                                {left - 1, bottomOut.y + 2},
                                {right + 1, bottomOut.y + 10 + 4},
                                outlineColor, 2.5, 1);

                            // armor bar

                            drawList->AddRectFilled(
                                {left - 1, bottomOut.y + 16},
                                {right + 1, bottomOut.y + 10 + 16},
                                ImColor{50,50,50, 200}, 2.5);

                            drawList->AddRectFilled(
                                {left, bottomOut.y + 16},
                                {std::lerp(left, right, armorFrac), bottomOut.y + 10 + 16},
                                armorColor, 2.5);

                            drawList->AddRect(
                                {left - 1, bottomOut.y + 15},
                                {right + 1, bottomOut.y + 10 + 17},
                                outlineColor, 2.5, 1);
                        }
                    }
                }
            }
        }
        ImGui::End();

        ImGui::PopFont();

        ImGui::SetNextWindowPos({20, (static_cast<float>(h) / 2) - 30});
        ImGui::SetNextWindowSize({200, 150});
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor{25,25,25,128}.Value);
        ImGui::Begin("##Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        {
            if (globals::Core::localPlayer) {
                ImGui::TextUnformatted(fmt::format("IP: {}", interfaces->engineClient->GetNetChannelInfo()->GetAddress()).c_str());
                ImGui::TextColored(globals::Core::sendPacket ? ImVec4{0,255,0,255} : ImVec4{255,0,0,255}, fmt::format("In sync: {}", globals::Core::sendPacket).c_str());
                ImGui::TextColored(globals::Hacks::AntiAim::breakingLby ? ImVec4{0,255,0,255} : ImVec4{255,0,0,255}, fmt::format("LBY: {}", globals::Hacks::AntiAim::breakingLby).c_str());
                ImGui::TextUnformatted(fmt::format("Latency: {}ms", static_cast<int>((interfaces->engineClient->GetNetChannelInfo()->GetLatency(0) + interfaces->engineClient->GetNetChannelInfo()->GetLatency(1)) * 100)).c_str());
                ImGui::TextUnformatted(fmt::format("Velocity: {}u/s", globals::Core::localPlayer->GetVelocity().Length2D()).c_str());
            }
        }
        ImGui::PopStyleColor();
        ImGui::End();
    }

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
