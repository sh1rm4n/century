//
// Created by sh1rm4n on 5/23/26.
//

#ifndef CENTURY_SWAPWINDOW_HPP
#define CENTURY_SWAPWINDOW_HPP

#include <SDL_events.h>
#include <SDL_video.h>

#include <imgui_stdlib.h>

#include <polyhook2/Detour/x64Detour.hpp>

#include "../3rd/imgui/imgui_impl_opengl3.h"
#include "../3rd/imgui/imgui_impl_sdl2.h"

#include "../3rd/imgui/imanim/im_anim.h"

#include "../3rd/fonts/IconsFontAwesome6.h"
#include "../3rd/fonts/fa-solid-900.h"
#include "../3rd/fonts/firaCodeFont.hpp"
#include "../3rd/fonts/robotoFont.hpp"

#include "../sourcesdk/entities/IClientRenderable.hpp"
#include "../sourcesdk/entities/C_BaseCombatWeapon.hpp"
#include "../sourcesdk/entities/C_CollisionProperty.hpp"

#include "../core/menu.hpp"
#include "../core/utils.hpp"
#include "../hacks/BackTracking.hpp"

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

        temp.IniFilename = nullptr;
        temp.LogFilename = nullptr;

        float baseFontSize = 16.0f;
        float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

        temp.Fonts->AddFontFromMemoryTTF(robotoFont, sizeof(robotoFont), baseFontSize, nullptr, temp.Fonts->GetGlyphRangesCyrillic());

        menu::pages::lua::editorFont = temp.Fonts->AddFontFromMemoryTTF(firaCodeFont, sizeof(firaCodeFont), baseFontSize, nullptr, temp.Fonts->GetGlyphRangesCyrillic());

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

    io.DisplaySize = {static_cast<float>(w), static_cast<float>(h)};

    ImGui::NewFrame();

    iam_update_begin_frame();
    iam_clip_update(io.DeltaTime);

    if (interfaces->engineClient->IsInGame()) {
        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize({static_cast<float>(w), static_cast<float>(h)});

        ImGui::Begin("##Background", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
        {
            const auto maxEntities = interfaces->entityList->GetMaxEntities();

            globals::Hacks::EntityESP::entsToDraw.clear();
            globals::Hacks::EntityESP::entsToDraw.reserve(maxEntities);

            const auto drawList = ImGui::GetBackgroundDrawList();

            if (globals::Hacks::EntityESP::enabled) {
                for (auto i = 0; i < maxEntities; i++) {
                    const auto entity = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

                    if (!entity || entity->IsPlayer() || entity->IsWeapon())
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

                        const auto origin = entity->GetNetworkedOrigin();

                        if (utils::WorldToScreen(origin, outOrigin)) {
                            const auto mins      = entity->GetCollisionProp()->OBBMins();
                            const auto maxs      = entity->GetCollisionProp()->OBBMaxs();
                            const auto health    = entity->GetHealth();
                            const auto maxHealth = std::max(entity->GetMaxHealth(), 1);
                            const auto model     = entity->GetModel();
                            const char* name     = entity->UsesLua() ? luaName : networkedName;

                            utils::Draw3DBox(drawList, origin, entity->GetNetworkedAngles(), mins, maxs);

                            // get 2D bounding rect from all 8 projected corners
                            Vector forward, right, up;
                            entity->GetNetworkedAngles().Vectors(forward, right, up);

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

            if (globals::Hacks::PlayerESP::bones ||
                globals::Hacks::PlayerESP::enabled)
            {
                for (auto i = 0; i <= interfaces->globalVars->maxClients; i++) {
                    const auto entity = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

                    if (!entity || entity == utils::GetLocalPlayer() || !entity->IsPlayer() || !entity->IsAlive())
                        continue;

                    player_info_s info;
                    if (!interfaces->engineClient->GetPlayerInfo(i, &info))
                        continue;

                    const auto head = entity->GetEyePosition();
                    const auto bottom = entity->GetNetworkedOrigin();

                    if (globals::Hacks::PlayerESP::bones) {
                        Matrix3x4 bones[256];
                        if (!entity->SetupBones(bones, 256, 0x7FF00, interfaces->globalVars->curTime))
                            continue;

                        utils::DrawBones(drawList, bones);
                    }

                    if (globals::Hacks::PlayerESP::renderingType == PlayerESPRenderingType::Square) {
                        Vector bottomOut;
                        Vector topOut;

                        const auto bottom = entity->GetNetworkedOrigin();
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
                    else if (globals::Hacks::PlayerESP::renderingType == PlayerESPRenderingType::Box) {
                        Vector headOut, bottomOut;

                        if (!utils::WorldToScreen(head + Vector{0, 0, 15}, headOut) ||
                            !utils::WorldToScreen(bottom, bottomOut))
                            continue;

                        // Box dimensions based on screen projection
                        float height = bottomOut.y - headOut.y;
                        float width  = height * 0.45f;

                        float x = bottomOut.x - width * 0.5f;
                        float y = headOut.y;

                        float x2 = bottomOut.x + width * 0.5f;
                        float y2 = bottomOut.y;

                        // ---------------- BOX ----------------
                        utils::Draw3DBox(drawList, bottom, entity->GetNetworkedAngles(), entity->GetCollisionProp()->OBBMins(), entity->GetCollisionProp()->OBBMaxs());

                        // ---------------- NAME ----------------
                        const auto weapon = entity->GetActiveWeapon();
                        auto weaponName = "";

                        if (weapon) {
                            weaponName = weapon->GetName();
                        }
                        else {
                            weaponName = "none";
                        }

                        ImVec2 weaponNameSize = ImGui::CalcTextSize(weaponName);
                        drawList->AddText(
                            ImVec2(bottomOut.x - weaponNameSize.x * 0.5f, y - weaponNameSize.y - 2.f),
                            IM_COL32(255, 255, 255, 255),
                            weaponName
                        );

                        ImVec2 nameSize = ImGui::CalcTextSize(info.name);
                        drawList->AddText(
                            ImVec2(bottomOut.x - nameSize.x * 0.5f, y - nameSize.y - 14.f),
                            IM_COL32(255, 255, 255, 255),
                            info.name
                        );

                        // ---------------- STEAM / ID LABEL ----------------
                        ImVec2 idSize = ImGui::CalcTextSize(info.guid);
                        drawList->AddText(
                            ImVec2(bottomOut.x - idSize.x * 0.5f, y2 + 8.f),
                            IM_COL32(180, 180, 180, 255),
                            info.guid
                        );

                        // ---------------- HEALTH BAR ----------------
                        float hp = std::clamp(static_cast<float>(entity->GetHealth()), 0.0f, 100.0f) / 100.0f;

                        float barW = width;
                        float barH = 4.0f;

                        ImVec2 barMin(x, y2 + 4.0f);       // bottom of box + padding
                        ImVec2 barMax(x + barW, y2 + 4.0f + barH);

                        // background
                        drawList->AddRectFilled(barMin, barMax, IM_COL32(20,20,20,220));

                        // fill (left → right)
                        drawList->AddRectFilled(
                            barMin,
                            ImVec2(barMin.x + barW * hp, barMax.y),
                            IM_COL32(
                                (int)(255 * (1.0f - hp)),
                                (int)(255 * hp),
                                0,
                                255
                            )
                        );
                    }
                }
            }
        }
        ImGui::End();

        ImGui::SetNextWindowPos({20, (static_cast<float>(h) / 2) - 30});
        ImGui::SetNextWindowSize({200, 100});
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor{25,25,25,128}.Value);
        ImGui::Begin("##Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        {
            if (globals::Core::localPlayer) {
                ImGui::Text(fmt::format("IP: {}", interfaces->engineClient->GetNetChannelInfo()->GetAddress()).c_str());
                ImGui::TextColored(globals::Core::sendPacket ? ImVec4{0,255,0,255} : ImVec4{255,0,0,255}, fmt::format("bSendPacket: {}", globals::Core::sendPacket).c_str());
                ImGui::Text(fmt::format("Latency: {}ms", static_cast<int>(interfaces->engineClient->GetNetChannelInfo()->GetLatency(0) * 693)).c_str());
                ImGui::Text(fmt::format("Velocity: {}u/s", globals::Core::localPlayer->GetVelocity().Length2D()).c_str());
            }
        }
        ImGui::PopStyleColor();
        ImGui::End();
    }

    if (globals::Menu::active) {
        SDL_Event e = {};

        while (SDL_PollEvent(&e)) {
            if (e.key.keysym.sym == SDLK_HOME && e.type == SDL_KEYDOWN) {
                globals::Menu::active = false;
            }
            else if (e.key.keysym.sym == SDLK_END && e.type == SDL_KEYDOWN){
                globals::Core::shouldUnload = true;
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
