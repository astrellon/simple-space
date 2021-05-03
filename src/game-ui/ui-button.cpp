#include "ui-button.hpp"

#include "./game-ui-manager.hpp"
#include "./ui-text-element.hpp"

#include "../render_camera.hpp"

namespace space
{
    void UIButton::init(GameUIManager &uiManager)
    {
        _textElement = uiManager.createElement<UITextElement>();
        _textElement->text("Button");

        addChild(_textElement);

        _backPanel.setFillColor(sf::Color::Blue);
        _backPanel.setOutlineColor(sf::Color::White);
        _backPanel.setOutlineThickness(1.0f);
        width(80);
        height(24);

        alignContent(YGAlignCenter);
    }

    void UIButton::drawSelf(Engine &engine, RenderCamera &target)
    {
        _backPanel.setSize(getSize());
        target.texture().draw(_backPanel, _transform);
    }
} // space