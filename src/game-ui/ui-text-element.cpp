#include "ui-text-element.hpp"

#include "../render_camera.hpp"
#include "../debug/draw_debug.hpp"

namespace space
{
    void UITextElement::draw(Engine &engine, RenderCamera &target)
    {
        _textElement.setCharacterSize(DrawDebug::fontSize);
        target.texture().draw(_textElement, _worldTransform);
    }
} // space