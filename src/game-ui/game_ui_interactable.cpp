#include "game_ui_interactable.hpp"

#include "../game/interactions/interactable.hpp"
#include "../engine.hpp"

#include "./ui_text_element.hpp"
#include "./ui_button.hpp"
#include "./game_ui_manager.hpp"

namespace space
{
    void GameUIInteractable::init(GameUIManager &uiManager)
    {
        _text = uiManager.createElement<UITextElement>();
        _uiManager = &uiManager;

        _text->widthPercent(100);
        _text->flexShrink(1.0f);
        addChild(_text);

        flexDirection(YGFlexDirectionRow);
    }

    void GameUIInteractable::interactable(Interactable *interactable)
    {
        auto uiManager = _uiManager;

        _interactable = interactable;
        _text->text(interactable->name());
        for (auto &action : interactable->interactions())
        {
            auto actionPtr = action.get();
            auto button = _uiManager->createElement<UIButton>();
            button->width(60);
            button->height(24);
            button->text(action->label());
            addChild(button);

            button->on(sf::Event::EventType::MouseButtonPressed, [actionPtr, uiManager] (const sf::Event &e)
            {
                actionPtr->execute(*uiManager->engine().currentSession());
                return UIEventResult::Triggered;
            });
        }
    }
} // space