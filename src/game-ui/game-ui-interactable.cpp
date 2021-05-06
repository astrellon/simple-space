#include "game-ui-interactable.hpp"

#include "../game/interactions/interactable.hpp"
#include "../engine.hpp"

#include "./ui-text-element.hpp"
#include "./ui-button.hpp"
#include "./game-ui-manager.hpp"

namespace space
{
    void GameUIInteractable::init(GameUIManager &uiManager)
    {
        _text = uiManager.createElement<UITextElement>();
        _uiManager = &uiManager;

        _text->widthPercent(100);
        _text->height(24);
        _text->flexShrink(1.0f);
        addChild(_text);
    }

    void GameUIInteractable::interactable(Interactable *interactable)
    {
        auto uiManager = _uiManager;

        _interactable = interactable;
        for (auto &action : interactable->interactions())
        {
            auto actionPtr = action.get();
            auto button = _uiManager->createElement<UIButton>();
            button->width(60);
            button->height(24);
            button->text(action->label());

            button->on(sf::Event::EventType::MouseButtonPressed, [actionPtr, uiManager] (const sf::Event &e)
            {
                actionPtr->execute(*uiManager->engine().currentSession());
                return UIEventResult::Triggered;
            });
        }
    }
} // space