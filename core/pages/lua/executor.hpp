//
// Created by sh1rm4n on 6/13/26.
//

#ifndef CENTURY_EXECUTOR_HPP
#define CENTURY_EXECUTOR_HPP

#include "../../../3rd/imgui/coloredtextbox/TextEditor.h"

namespace menu::pages::executor {
    void Render() noexcept {
        static auto editor = std::make_unique<TextEditor>();

        if (ImGui::Button("Run"))
            luaExecution->Execute(LUA_CLIENT, editor->GetText());

        ImGui::SameLine();

        editor->SetLanguage(TextEditor::Language::Lua());
        editor->Render("Editor");
    }
}

#endif //CENTURY_EXECUTOR_HPP
