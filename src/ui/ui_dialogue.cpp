#include "ui_dialogue.hpp"

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../dialogue_manager.hpp"
#include "../engine.hpp"
#include "../game_session.hpp"

namespace space
{
    UIDialogue::UIDialogue() : UIWindow("Dialogue")
    {
        size = ImVec2(400, 120);
    }

    void UIDialogue::checkOpen(Engine &engine)
    {
        if (!engine.currentSession())
        {
            isOpen = false;
            return;
        }

        isOpen = engine.currentSession()->dialogueManager().isInDialogue();
    }

    void UIDialogue::checkPosition(Engine &engine)
    {
        auto renderSize = engine.renderSize();
        position.x = (renderSize.x - size.x) * 0.5;
        position.y = renderSize.y - size.y - 20;
    }

    void UIDialogue::doDraw(Engine &engine)
    {
        auto &manager = engine.currentSession()->dialogueManager();
        ImGui::Text("%s", manager.currentLine().c_str());
        if (ImGui::Button("Next"))
        {
            manager.nextLine();
        }
    }
} // namespace space
