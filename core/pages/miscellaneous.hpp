//
// Created by sh1rm4n on 6/5/26.
//

#ifndef CENTURY_NETWORK_HPP
#define CENTURY_NETWORK_HPP

#include "../globals.hpp"

#include <imgui.h>

#include "../interfaces.hpp"

namespace menu::pages::miscellaneous {
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
        ImGui::Checkbox("Engine prediction", &globals::core::enginePrediction);

        if (ImGui::BeginChild("##FakeLag", ImVec2(0.f, 0.f), ImGuiChildFlags_Borders)) {
            static const char* methods[] = {"bSendPacket\0", "Choking\0", "bSendPacket + Choking\0"};

            ImGui::SeparatorText("Fake Lag");

            ImGui::Checkbox("Enabled?", &globals::hacks::fakelag::enabled);
            ImGui::Combo("Lag method", &globals::hacks::fakelag::method, methods, IM_ARRAYSIZE(methods));
            ImGui::SliderInt("Lag ticks", &globals::hacks::fakelag::ticks, 1, 14);

            ImGui::EndChild();
        }
    }
}

#endif //CENTURY_NETWORK_HPP
