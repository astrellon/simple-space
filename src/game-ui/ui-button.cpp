#include "ui-button.hpp"

#include "./game-ui-manager.hpp"
#include "./ui-text-element.hpp"
#include "./ui-nine-slice-image-element.hpp"

#include "../render_camera.hpp"

namespace space
{
    void UIButton::init(GameUIManager &uiManager)
    {
        _backPanel = uiManager.createElement<UINineSliceImageElement>();
        _backPanel->nineSlice(uiManager.defaultButton());
        _backPanel->positionType(YGPositionTypeAbsolute);
        _backPanel->widthPercent(100);
        _backPanel->heightPercent(100);
        addChild(_backPanel);

        _textElement = uiManager.createElement<UITextElement>();
        _textElement->text("Button");
        _textElement->margin(YGEdgeLeft, 8);
        addChild(_textElement);

        width(80);
        height(24);

        alignContent(YGAlignCenter);

        on(sf::Event::EventType::MouseEntered, [this](const sf::Event &event)
        {
            this->_backPanel->nineSlice().colour(sf::Color(255, 255, 255, 200));
            return UIEventResult::Triggered;
        });

        on(sf::Event::EventType::MouseLeft, [this](const sf::Event &event)
        {
            this->_backPanel->nineSlice().colour(sf::Color::White);
            return UIEventResult::Triggered;
        });
    }

    void UIButton::text(const std::string &text)
    {
        _textElement->text(text);
    }
    std::string UIButton::text() const
    {
        return _textElement->text();
    }
} // space