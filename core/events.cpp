//
// Created by sh1rm4n on 7/9/26.
//

#include "events.hpp"

#include <fmt/format.h>

#include "globals.hpp"
#include "interfaces.hpp"
#include "utils.hpp"
#include "../3rd/imgui/imgui_notify/ImGuiNotify.hpp"
#include "../hacks/Resolver.hpp"
#include "../sourcesdk/entities/C_BaseCombatWeapon.hpp"

static std::string ReplaceVariables(
    std::string text,
    std::initializer_list<std::pair<std::string_view, std::string_view>> vars)
{
    for (const auto& [name, value] : vars) {
        const std::string pattern = fmt::format("{{{}}}", name);

        size_t pos = 0;
        while ((pos = text.find(pattern, pos)) != std::string::npos) {
            text.replace(pos, pattern.size(), value);
            pos += value.size();
        }
    }

    return text;
}

class PlayerHurtListener : public IGameEventListener2 {
public:
    void FireGameEvent(IGameEvent *event) override {
        const auto attackerId = interfaces->engineClient->GetPlayerForUserID(event->GetInt("attacker"));
        const auto victimId = interfaces->engineClient->GetPlayerForUserID(event->GetInt("userid"));

        const auto remainingHealth = event->GetInt("health");

        const auto victim = interfaces->entityList->GetClientEntity<C_BaseEntity>(victimId);
        if (!victim)
            return;

        player_info_s attackerInfo{};
        if (!interfaces->engineClient->GetPlayerInfo(attackerId, &attackerInfo))
            return;

        player_info_s victimInfo{};
        if (!interfaces->engineClient->GetPlayerInfo(victimId, &victimInfo))
            return;

        if (attackerId == 0) {
            return;
        }

        if (attackerId == interfaces->engineClient->GetLocalPlayer()) {
            interfaces->engineClient->ClientCmd_Unrestricted("play plats/elevbell1.wav");

            ImGui::InsertNotification({ImGuiToastType::Success, fmt::format("{} now has {}HP!", victimInfo.name, remainingHealth).c_str()});

            return;
        }
    }
};

class PlayerDeathListener : public IGameEventListener2 {
public:
    void FireGameEvent(IGameEvent *event) override {
        const auto attackerId = event->GetInt("entindex_attacker");
        const auto victimId = event->GetInt("entindex_killed");

        const auto victim = interfaces->entityList->GetClientEntity<C_BaseEntity>(victimId);
        if (!victim)
            return;

        player_info_s attackerInfo{};
        if (!interfaces->engineClient->GetPlayerInfo(attackerId, &attackerInfo))
            return;

        player_info_s victimInfo{};
        if (!interfaces->engineClient->GetPlayerInfo(victimId, &victimInfo))
            return;

        if (resolver->ContainsPlayer(victim))
            resolver->RemovePlayer(victim);

        if (attackerId == 0) {
            interfaces->engineClient->ClientCmd_Unrestricted("say упал отжался");
            return;
        }

        if (globals::Hacks::Killfeed::enabled) {
            if (attackerId == interfaces->engineClient->GetLocalPlayer()) {
                const auto& randomWord = globals::Hacks::Killfeed::words[
                    utils::Random<float>(0, globals::Hacks::Killfeed::words.size())];

                const auto message = ReplaceVariables(randomWord,{{"attacker", attackerInfo.name},{"victim", victimInfo.name}});

                interfaces->engineClient->ClientCmd_Unrestricted(fmt::format("say {}", message).c_str());

                return;
            }
        }
    }
};

void Events::Prepare() noexcept {
    playerHurt = new PlayerHurtListener();
    playerDeath = new PlayerDeathListener();

    interfaces->gameEventsManager->AddListener(playerHurt, "player_hurt", false);
    interfaces->gameEventsManager->AddListener(playerDeath, "entity_killed", false);
}

void Events::Unload() noexcept {
    interfaces->gameEventsManager->RemoveListener(playerHurt);
    interfaces->gameEventsManager->RemoveListener(playerDeath);
}
