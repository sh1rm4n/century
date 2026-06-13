//
// Created by sh1rm4n on 5/23/26.
//

#ifndef CENTURY_SWAPWINDOW_HPP
#define CENTURY_SWAPWINDOW_HPP

#include <SDL_events.h>
#include <SDL_video.h>

#include <polyhook2/Detour/x64Detour.hpp>

#include "../3rd/imgui/imgui_impl_opengl3.h"
#include "../3rd/imgui/imgui_impl_sdl2.h"

#include "../3rd/imgui/imanim/im_anim.h"

#include "../3rd/fonts/IconsFontAwesome6.h"
#include "../3rd/fonts/fa-solid-900.h"

#include <imgui_stdlib.h>

#include "../core/menu.hpp"

inline std::uint64_t ogSwapWindow = 0;

void dtSwapWindow(SDL_Window* window) {
    static auto ogFunction = PLH::FnCast(ogSwapWindow, dtSwapWindow);

    static bool init = false;

    static void* mainContext = SDL_GL_GetCurrentContext();
    static void* customContext = nullptr;

    if (!init) {
        customContext = SDL_GL_CreateContext(window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        menu::Setup();

        ImGui_ImplOpenGL3_Init();
        ImGui_ImplSDL2_InitForOpenGL(window, nullptr);

        auto& temp = ImGui::GetIO();

        temp.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        temp.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        temp.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        temp.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        temp.Fonts->AddFontDefault();

        float baseFontSize = 16.0f;
        float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

        static constexpr ImWchar iconsRanges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphMinAdvanceX = iconFontSize;
        temp.Fonts->AddFontFromMemoryTTF(fa_solid_900, sizeof(fa_solid_900), iconFontSize, &iconsConfig, iconsRanges);

        init = true;
    }

    static ImGuiIO& io = ImGui::GetIO();

    SDL_GL_MakeCurrent(window, customContext);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    globals::core::screenWidth = static_cast<float>(w);
    globals::core::screenHeight = static_cast<float>(h);

    io.DisplaySize = {static_cast<float>(w), static_cast<float>(h)};

    ImGui::NewFrame();

    iam_update_begin_frame();
    iam_clip_update(io.DeltaTime);

    if (interfaces->engineClient->IsInGame()) {
        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize({globals::core::screenWidth, globals::core::screenHeight});
        ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
        {
            const auto maxEntities = interfaces->entityList->GetMaxEntities();

            globals::hacks::entityesp::entsToDraw.clear();
            globals::hacks::entityesp::entsToDraw.reserve(maxEntities);

            const auto drawList = ImGui::GetBackgroundDrawList();

            if (globals::hacks::entityesp::enabled) {
                for (auto i = 0; i < maxEntities; i++) {
                    const auto entity = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

                    if (!entity || entity->IsPlayer() || entity->IsWeapon())
                        continue;

                    const auto clientClass = entity->GetClientClass();
                    if (!clientClass)
                        continue;

                    const auto networkedName = clientClass->m_pNetworkName;
                    const auto luaName = entity->GetLuaScriptName();

                    // 1. Construct the identifier string once
                    std::string entIdentifier = std::to_string(i) + ": " + ((luaName && luaName[0] != '\0') ? luaName : networkedName);

                    globals::hacks::entityesp::entsToDraw.emplace_back(DrawEntity{entIdentifier, entity->UsesLua()});

                    // 2. Check if the generated identifier string is allowed to be drawn
                    if (globals::hacks::entityesp::allowedEntsToDraw.contains(entIdentifier)) {
                        if (!globals::core::localPlayer)
                            continue;

                        Vector outOrigin;

                        const auto origin = entity->GetAbsOrigin();

                        if (utils::WorldToScreen(origin, outOrigin)) {
                            if (entity->UsesLua()) {
                                drawList->AddText({outOrigin.x, outOrigin.y}, ImColor{255, 255, 255}, luaName);
                            }
                            else {
                                drawList->AddText({outOrigin.x, outOrigin.y}, ImColor{255, 255, 255}, networkedName);
                            }

                            const auto model = entity->GetModel();
                            const auto health = entity->GetHealth();
                            const auto maxHealth = entity->GetMaxHealth();

                            float healthPercent = static_cast<float>(health) / maxHealth;
                            healthPercent = std::clamp(healthPercent, 0.0f, 1.0f);

                            const auto r = static_cast<int>((1.0f - healthPercent) * 255.0f);
                            const auto g = static_cast<int>(healthPercent * 255.0f);

                            ImColor healthColor(r, g, 0);

                            if (model != nullptr)
                                drawList->AddText({outOrigin.x, outOrigin.y - 13.f}, ImColor{255, 255, 255}, std::format("Model: {}", model->strName).c_str());
                            else
                                drawList->AddText({outOrigin.x, outOrigin.y - 13.f}, ImColor{255, 255, 255}, "Model: none");

                            drawList->AddText({outOrigin.x, outOrigin.y - 26.f}, healthColor, std::format("Health: {}/{}", health, maxHealth).c_str());

                            drawList->AddText({outOrigin.x, outOrigin.y - 39.f}, ImColor{0, 255, 247}, "Entity");

                            utils::Draw3DBox(drawList, origin, entity->GetAbsAngles(), entity->WorldAlignMins(), entity->WorldAlignMaxs());
                            //utils::Warn("ent: {}; mins.x: {}; mins.y: {}; mins.z", luaName, mins.x, mins.y, mins.z);
                        }
                    }
                }
            }

            if (globals::hacks::playeresp::bones ||
                globals::hacks::playeresp::enabled)
            {
                for (auto i = 0; i <= interfaces->globalVars->maxClients; i++) {
                    const auto entity = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

                    if (!entity || entity == utils::GetLocalPlayer() || !entity->IsPlayer() || !entity->IsAlive())
                        continue;

                    utils::PushAllowBoneAccess(true, false, "century_player_esp");

                    Matrix3x4 bones[256];
                    if (!entity->SetupBones(bones, 256, 0x7FF00, interfaces->globalVars->curtime))
                        continue;

                    utils::PopBoneAccess("century_player_esp");

                    player_info_s info;
                    if (!interfaces->engineClient->GetPlayerInfo(i, &info))
                        continue;

                    const auto head = bones[7].GetOrigin();

                    if (globals::hacks::playeresp::bones) {
                        // pelvis -> spines -> head
                        Vector bone0 = bones[0].GetOrigin();
                        Vector bone1 = bones[1].GetOrigin();
                        Vector bone2 = bones[2].GetOrigin();
                        Vector bone3 = bones[3].GetOrigin();
                        Vector bone4 = bones[4].GetOrigin();
                        Vector bone5 = bones[5].GetOrigin();
                        Vector bone6 = bones[6].GetOrigin();
                        Vector bone7 = head;

                        // shoulders -> arms -> hands

                        // left arm
                        Vector bone8 = bones[8].GetOrigin();
                        Vector bone9 = bones[9].GetOrigin();
                        Vector bone10 = bones[10].GetOrigin();
                        Vector bone11 = bones[11].GetOrigin();
                        Vector bone12 = bones[12].GetOrigin();

                        // right arm
                        Vector bone13 = bones[13].GetOrigin();
                        Vector bone14 = bones[14].GetOrigin();
                        Vector bone15 = bones[15].GetOrigin();
                        Vector bone16 = bones[16].GetOrigin();
                        Vector bone17 = bones[17].GetOrigin();

                        // pelvis -> legs

                        // left leg
                        Vector bone18 = bones[18].GetOrigin();
                        Vector bone19 = bones[19].GetOrigin();
                        Vector bone20 = bones[20].GetOrigin();
                        Vector bone21 = bones[21].GetOrigin();

                        // right leg
                        Vector bone22 = bones[22].GetOrigin();
                        Vector bone23 = bones[23].GetOrigin();
                        Vector bone24 = bones[24].GetOrigin();
                        Vector bone25 = bones[25].GetOrigin();

                        Vector outBone0;
                        Vector outBone1;
                        Vector outBone2;
                        Vector outBone3;
                        Vector outBone4;
                        Vector outBone5;
                        Vector outBone6;
                        Vector outBone7;

                        Vector outBone8;
                        Vector outBone9;
                        Vector outBone10;
                        Vector outBone11;
                        Vector outBone12;
                        Vector outBone13;
                        Vector outBone14;
                        Vector outBone15;
                        Vector outBone16;
                        Vector outBone17;

                        Vector outBone18;
                        Vector outBone19;
                        Vector outBone20;
                        Vector outBone21;

                        Vector outBone22;
                        Vector outBone23;
                        Vector outBone24;
                        Vector outBone25;

                        if (utils::WorldToScreen(bone0, outBone0) &&
                            utils::WorldToScreen(bone1, outBone1) &&
                            utils::WorldToScreen(bone2, outBone2) &&
                            utils::WorldToScreen(bone3, outBone3) &&
                            utils::WorldToScreen(bone4, outBone4) &&
                            utils::WorldToScreen(bone5, outBone5) &&
                            utils::WorldToScreen(bone6, outBone6) &&
                            utils::WorldToScreen(bone7, outBone7) &&
                            utils::WorldToScreen(bone8, outBone8) &&
                            utils::WorldToScreen(bone9, outBone9) &&
                            utils::WorldToScreen(bone10, outBone10) &&
                            utils::WorldToScreen(bone11, outBone11) &&
                            utils::WorldToScreen(bone12, outBone12) &&
                            utils::WorldToScreen(bone13, outBone13) &&
                            utils::WorldToScreen(bone14, outBone14) &&
                            utils::WorldToScreen(bone15, outBone15) &&
                            utils::WorldToScreen(bone16, outBone16) &&
                            utils::WorldToScreen(bone17, outBone17) &&
                            utils::WorldToScreen(bone18, outBone18) &&
                            utils::WorldToScreen(bone19, outBone19) &&
                            utils::WorldToScreen(bone20, outBone20) &&
                            utils::WorldToScreen(bone21, outBone21) &&
                            utils::WorldToScreen(bone22, outBone22) &&
                            utils::WorldToScreen(bone23, outBone23) &&
                            utils::WorldToScreen(bone24, outBone24) &&
                            utils::WorldToScreen(bone25, outBone25))
                        {
                            drawList->AddLine({outBone0.x, outBone0.y}, {outBone1.x, outBone1.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone1.x, outBone1.y}, {outBone2.x, outBone2.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone2.x, outBone2.y}, {outBone3.x, outBone3.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone3.x, outBone3.y}, {outBone4.x, outBone4.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone4.x, outBone4.y}, {outBone5.x, outBone5.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone5.x, outBone5.y}, {outBone6.x, outBone6.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone6.x, outBone6.y}, {outBone7.x, outBone7.y}, ImColor{255,255,255});

                            drawList->AddLine({outBone4.x, outBone4.y}, {outBone8.x, outBone8.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone8.x, outBone8.y}, {outBone9.x, outBone9.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone9.x, outBone9.y}, {outBone10.x, outBone10.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone10.x, outBone10.y}, {outBone11.x, outBone11.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone11.x, outBone11.y}, {outBone12.x, outBone12.y}, ImColor{255,255,255});

                            drawList->AddLine({outBone4.x, outBone4.y}, {outBone13.x, outBone13.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone13.x, outBone13.y}, {outBone14.x, outBone14.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone14.x, outBone14.y}, {outBone15.x, outBone15.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone15.x, outBone15.y}, {outBone16.x, outBone16.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone16.x, outBone16.y}, {outBone17.x, outBone17.y}, ImColor{255,255,255});

                            drawList->AddLine({outBone0.x, outBone0.y}, {outBone18.x, outBone18.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone18.x, outBone18.y}, {outBone19.x, outBone19.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone19.x, outBone19.y}, {outBone20.x, outBone20.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone20.x, outBone20.y}, {outBone21.x, outBone21.y}, ImColor{255,255,255});

                            drawList->AddLine({outBone0.x, outBone0.y}, {outBone22.x, outBone22.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone22.x, outBone22.y}, {outBone23.x, outBone23.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone23.x, outBone23.y}, {outBone24.x, outBone24.y}, ImColor{255,255,255});
                            drawList->AddLine({outBone24.x, outBone24.y}, {outBone25.x, outBone25.y}, ImColor{255,255,255});
                        }
                    }

                    if (globals::hacks::playeresp::renderingType == PlayerESPRenderingType::Square) {
                        Vector bottomOut;
                        Vector topOut;

                        const auto bottom = entity->GetAbsOrigin();
                        const auto top = head + Vector{0.f,0.f,11.f};

                        if (utils::WorldToScreen(bottom, bottomOut) && utils::WorldToScreen(top, topOut)) {
                            const auto height = bottomOut.y - topOut.y;
                            const auto width = height * 0.35f;

                            const auto left = topOut.x - width;
                            const auto right = topOut.x + width;

                            drawList->AddText({topOut.x, topOut.y}, ImColor{255,255,255}, info.name);

                            drawList->AddRect({left + 1.f, topOut.y + 1.f}, {right + 1.f, bottomOut.y + 1.f}, ImColor{35,35,35});
                            drawList->AddRect({left, topOut.y}, {right, bottomOut.y}, ImColor{255,255,255});
                            drawList->AddRect({left - 1.f, topOut.y - 1.f}, {right - 1.f, bottomOut.y - 1.f}, ImColor{35,35,35});
                        }
                    }
                    else if (globals::hacks::playeresp::renderingType == PlayerESPRenderingType::Box) {
                        Vector headOut;

                        if (!utils::WorldToScreen(head, headOut))
                            continue;

                        drawList->AddText({headOut.x, headOut.y}, ImColor{255,255,255}, info.name);

                        utils::Draw3DBox(drawList, entity->GetAbsOrigin(), entity->GetAbsAngles(), entity->WorldAlignMins(), entity->WorldAlignMaxs());
                    }
                }
            }
        }
        ImGui::End();
    }

    if (globals::menu::active) {
        SDL_Event e = {};

        while (SDL_PollEvent(&e)) {
            if (e.key.keysym.sym == SDLK_HOME && e.type == SDL_KEYDOWN) {
                globals::menu::active = false;
            }
            else if (e.key.keysym.sym == SDLK_END && e.type == SDL_KEYDOWN){
                globals::core::shouldUnload = true;
            }

            ImGui_ImplSDL2_ProcessEvent(&e);
        }

        menu::Render();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 1.00f));

    ImGui::RenderNotifications();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(1);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    ogFunction(window);

    SDL_GL_MakeCurrent(window, mainContext);
}

#endif //CENTURY_SWAPWINDOW_HPP
