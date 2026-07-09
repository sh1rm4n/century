//
// Created by sh1rm4n on 7/7/26.
//

#ifndef CENTURY_FRIENDS_HPP
#define CENTURY_FRIENDS_HPP

#include "../globals.hpp"
#include "../interfaces.hpp"

#include "../../sourcesdk/entities/C_BaseEntity.hpp"

namespace menu::pages::friends {
    void Render() noexcept {
        static std::string searchFor;

        ImGui::InputText("Search for enemies/friends", &searchFor);

        if (ImGui::BeginListBox("Enemies")) {
            for (auto i = 0; i <= interfaces->globalVars->maxClients; i++) {
                auto player = interfaces->entityList->GetClientEntity<C_BaseEntity>(i);

                if (!player || player == globals::core::localPlayer || !player->IsPlayer())
                    continue;

                player_info_s info{};
                if (!interfaces->engineClient->GetPlayerInfo(i, &info))
                    continue;

                if (globals::core::friends.contains(info.name))
                    continue;

                std::string name = info.name;
                std::string steamID = info.guid;
                std::string label = std::format("{} ({})", name, steamID);

                if (!searchFor.empty() && !label.contains(searchFor))
                    continue;

                if (ImGui::Selectable(label.c_str()))
                    globals::core::friends.emplace(name, steamID);
            }

            ImGui::EndListBox();
        }

        if (ImGui::BeginListBox("Friends")) {
            for (auto it = globals::core::friends.begin(); it != globals::core::friends.end();) {
                std::string label = std::format("{} ({})", it->first, it->second);

                if (!searchFor.empty() && !label.contains(searchFor)) {
                    ++it;
                    continue;
                }

                if (ImGui::Selectable(label.c_str()))
                    it = globals::core::friends.erase(it);
                else
                    ++it;
            }
        }
    }
}

#endif //CENTURY_FRIENDS_HPP
