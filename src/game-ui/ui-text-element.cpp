#include "ui-text-element.hpp"

#include "../render_camera.hpp"
#include "../debug/draw_debug.hpp"

#include "./game-ui-manager.hpp"

namespace space
{
    void UITextElement::init(GameUIManager &uiManager)
    {
        font(uiManager.defaultFont());
    }

    void UITextElement::drawSelf(Engine &engine, RenderCamera &target)
    {
        _textElement.setCharacterSize(DrawDebug::fontSize);
        target.texture().draw(_textElement, _transform);
    }
} // space