//
// Created by sh1rm4n on 7/7/26.
//

#ifndef CENTURY_FRIENDS_HPP
#define CENTURY_FRIENDS_HPP

#include "../globals.hpp"
#include "../interfaces.hpp"

#include <imgui_stdlib.h>

#include "../../sourcesdk/entities/C_BaseEntity.hpp"

namespace menu::pages::friends {
    void Render() noexcept {
        static std::string searchFor;

        ImGui::InputText("Search for enemies/friends", &searchFor);

        if (ImGui::BeginListBox("Enemies")) {
            for (auto i = 0; i <= interfaces->globalVars->maxClients; i++) {
                auto player = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

                if (!player || player == globals::Core::localPlayer || !player->IsPlayer())
                    continue;

                player_info_s info{};
                if (!interfaces->engineClient->GetPlayerInfo(i, &info))
                    continue;

                if (globals::Core::friends.contains(info.name))
                    continue;

                std::string name = info.name;
                std::string steamID = info.guid;
                std::string label = std::format("{} ({})", name, steamID);

                if (!searchFor.empty() && !label.contains(searchFor))
                    continue;

                if (ImGui::Selectable(label.c_str()))
                    globals::Core::friends.emplace(name, steamID);
            }

            ImGui::EndListBox();
        }

        if (ImGui::BeginListBox("Friends")) {
            for (auto it = globals::Core::friends.begin(); it != globals::Core::friends.end();) {
                std::string label = std::format("{} ({})", it->first, it->second);

                if (!searchFor.empty() && !label.contains(searchFor)) {
                    ++it;
                    continue;
                }

                if (ImGui::Selectable(label.c_str()))
                    it = globals::Core::friends.erase(it);
                else
                    ++it;
            }

            ImGui::EndListBox();
        }
    }
}

#endif //CENTURY_FRIENDS_HPP
