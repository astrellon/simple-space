#include "game_ui_interactables_panel.hpp"

#include "./game_ui_interactable.hpp"

#include "../game_ui_manager.hpp"
#include "../ui_text_element.hpp"
#include "../ui_button.hpp"
#include "../ui_panel.hpp"

#include "../../game/interactions/interactable.hpp"
#include "../../game/interactions/interactable_list.hpp"
#include "../../engine.hpp"
#include "../../game_session.hpp"
#include "../../controllers/player_controller.hpp"

namespace space
{
    void GameUIInteractablesPanel::init(GameUIManager &uiManager)
    {
        UIElement::init(uiManager);
        _uiManager = &uiManager;

        _panel = uiManager.createElement<UIPanel>();
        addChild(_panel);

        width(200);
        heightAuto();
        flexShrink(1.0f);
        positionType(YGPositionTypeAbsolute);
    }

    void GameUIInteractablesPanel::draw(Engine &engine, RenderCamera &target)
    {
        if (_interactables == nullptr || _interactables->interactables().size() == 0)
        {
            return;
        }

        UIElement::draw(engine, target);
    }

    void GameUIInteractablesPanel::preUpdate(Engine &engine, sf::Time dt)
    {
        if (destroyed())
        {
            return;
        }

        auto session = engine.currentSession();
        if (session)
        {
            if (session->playerController().controlling() == ControllingValue::ControlCharacter)
            {
                interactables(&session->playerController().canInteractWithInRange());
            }
            else
            {
                interactables(nullptr);
            }
        }
        else
        {
            interactables(nullptr);
        }

        UIElement::preUpdate(engine, dt);
    }

    void GameUIInteractablesPanel::update(Engine &engine, sf::Time dt, sf::Vector2f parentOffset)
    {
        auto screenSize = engine.renderSize();
        margin(screenSize.y / 2 - 32, 0, 0, screenSize.x / 2 + 32);

        UIElement::update(engine, dt, parentOffset);
    }

    void GameUIInteractablesPanel::interactables(const InteractableList *interactables)
    {
        if (interactables == _interactables)
        {
            return;
        }

        for (auto interactableUI : _interactableUIs)
        {
            _panel->bodyContainer()->removeChild(interactableUI);
            _uiManager->removeElement(interactableUI);
        }
        _interactableUIs.clear();

        _removeOnAddInteractable.reset();
        _removeOnRemoveInteractable.reset();

        _interactables = interactables;

        if (interactables)
        {
            for (auto interactable : interactables->interactables())
            {
                addInteractable(interactable);
            }

            _removeOnAddInteractable = interactables->onAddInteractable.createObserver([this](Interactable *interactable)
            {
                std::cout << "Added interactable" << std::endl;
                this->addInteractable(interactable);
            });
            _removeOnRemoveInteractable = interactables->onRemoveInteractable.createObserver([this](Interactable *interactable)
            {
                std::cout << "Removed interactable" << std::endl;
                this->removeInteractable(interactable);
            });
        }
    }

    void GameUIInteractablesPanel::addInteractable(Interactable *interactable)
    {
        auto interactableUI = _uiManager->createElement<GameUIInteractable>();
        interactableUI->interactable(interactable);

        _panel->bodyContainer()->addChild(interactableUI);
        _interactableUIs.push_back(interactableUI);
    }

    void GameUIInteractablesPanel::removeInteractable(Interactable *interactable)
    {
        for (auto iter = _interactableUIs.begin(); iter != _interactableUIs.end(); ++iter)
        {
            auto interactableUI = *iter;
            if (interactableUI->interactable() == interactable)
            {
                _interactableUIs.erase(iter);
                _panel->bodyContainer()->removeChild(interactableUI);
                _uiManager->removeElement(interactableUI);
                break;
            }
        }
    }
} // space