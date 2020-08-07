#include "ui_dialogue.hpp"

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../dialogue_manager.hpp"

namespace space
{
    void UIDialogue::draw(DialogueManager &manager)
    {
        if (!manager.isInDialogue())
        {
            return;
        }

        ImGui::Begin("Dialogue");
        ImGui::Text("%s", manager.currentLine().c_str());
        if (ImGui::Button("Next"))
        {
            manager.nextLine();
        }
        ImGui::End();
    }
} // namespace space
