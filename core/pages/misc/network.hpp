//
// Created by sh1rm4n on 6/5/26.
//

#ifndef CENTURY_NETWORK_HPP
#define CENTURY_NETWORK_HPP

#include "../../../core/globals.hpp"

#include <imgui.h>

#include "../../interfaces.hpp"

namespace menu::pages::network {
    void Render() noexcept {
        static std::string reason = "imagine gmod cheating on arch :skull:";

        ImGui::InputText("Disconnect reason", &reason);
        if (ImGui::Button("Custom disconnect")) {
            const auto netchan = interfaces->engineClient->GetNetChannelInfo();

            if (netchan)
                netchan->Shutdown(reason.c_str());
            else
                ImGui::InsertNotification({ImGuiToastType::Error, "Please join any server."});
        }
    }
}

#endif //CENTURY_NETWORK_HPP
