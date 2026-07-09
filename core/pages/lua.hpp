//
// Created by sh1rm4n on 6/13/26.
//

#ifndef CENTURY_EXECUTOR_HPP
#define CENTURY_EXECUTOR_HPP

#include "../menu.hpp"
#include "../menu.hpp"
#include "../../3rd/imgui/coloredtextbox/TextEditor.h"

namespace menu::pages::lua {
    inline ImFont* editorFont = nullptr;

    void Render() noexcept {
        static std::unique_ptr<TextEditor> editor = nullptr;
        if (!editor) {
            editor = std::make_unique<TextEditor>();
            editor->SetLanguage(TextEditor::Language::Lua());
        }

        static constexpr std::array<std::string_view, 3> realms = { "Client", "Server", "Menu" };

        if (ImGui::Button("Execute"))
            luaExecution->Execute(globals::core::luaRealm, editor->GetText());

        ImGui::SameLine();

        if (ImGui::BeginCombo("Realm", realms[globals::core::luaRealm].data())) {
            for (int i = 0; i < static_cast<int>(realms.size()); ++i) {
                if (ImGui::Selectable(realms[i].data(), globals::core::luaRealm == i))
                    globals::core::luaRealm = i;
                if (globals::core::luaRealm == i)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::PushFont(editorFont);
        editor->Render("Editor");
        ImGui::PopFont();
    }
}

#endif //CENTURY_EXECUTOR_HPP
