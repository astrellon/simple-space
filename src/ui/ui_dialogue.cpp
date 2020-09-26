#include "ui_dialogue.hpp"

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../dialogue_manager.hpp"
#include "../engine.hpp"
#include "../game_session.hpp"

#include "../mustache.hpp"

using kainjow::mustache::mustache;

namespace space
{
    UIDialogue::UIDialogue() : UIPanel("Dialogue"), _textDisplayIndex(0)
    {
        size = ImVec2(400, 120);
    }

    bool UIDialogue::isOpen(Engine &engine)
    {
        if (!engine.currentSession())
        {
            return false;
        }

        return show && engine.currentSession()->dialogueManager().isInDialogue();
    }

    void UIDialogue::checkPosition(Engine &engine)
    {
        auto renderSize = engine.renderSize();
        position.x = (renderSize.x - size.x) * 0.5;
        position.y = renderSize.y - size.y - 20;
    }

    void UIDialogue::doDraw(Engine &engine)
    {
        if (_text.size() == 0)
        {
            processText(engine);
        }
        else
        {
            if (_textDisplayIndex < _text.size())
            {
                _textDisplay[_textDisplayIndex] = _text[_textDisplayIndex];
                _textDisplayIndex++;
                _textDisplay[_textDisplayIndex] = '\0';
            }
        }

        auto &manager = engine.currentSession()->dialogueManager();
        ImGui::Text("%s:", manager.personTalkingName().c_str());
        ImGui::TextWrapped("%s", _textDisplay.data());
        if (ImGui::Button("Next"))
        {
            manager.nextLine();
            _text = "";
        }
    }

    void UIDialogue::processText(Engine &engine)
    {
        auto &manager = engine.currentSession()->dialogueManager();

        mustache tmplt{manager.currentLine()};
        _text = tmplt.render({"name", manager.personTalkingName()});

        _textDisplay.reserve(_text.size() + 1);
        _textDisplayIndex = 0;
        _textDisplay[0] = '\0';
    }
} // namespace space
