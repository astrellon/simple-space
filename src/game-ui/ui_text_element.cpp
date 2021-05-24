#include "ui_text_element.hpp"

#include "../render_camera.hpp"
#include "../debug/draw_debug.hpp"

#include "./game_ui_manager.hpp"

namespace space
{
    void UITextElement::init(GameUIManager &uiManager)
    {
        font(uiManager.defaultFont());

        widthAuto();
        height(24);
    }

    void UITextElement::drawSelf(Engine &engine, RenderCamera &target)
    {
        _textElement.setCharacterSize(DrawDebug::fontSize);
        target.texture().draw(_textElement, _transform);
    }
} // space