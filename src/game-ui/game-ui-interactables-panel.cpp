#include "game-ui-interactables-panel.hpp"

#include "./game-ui-manager.hpp"
#include "./ui-text-element.hpp"
#include "./ui-button.hpp"
#include "./game-ui-interactable.hpp"
#include "./ui-nine-slice-image-element.hpp"

#include "../game/interactions/interactable.hpp"
#include "../game/interactions/interactable_list.hpp"
#include "../engine.hpp"
#include "../game_session.hpp"
#include "../controllers/player_controller.hpp"

namespace space
{
    void GameUIInteractablesPanel::init(GameUIManager &uiManager)
    {
        UIElement::init(uiManager);
        _uiManager = &uiManager;

        _backPanel = uiManager.createElement<UINineSliceImageElement>();
        _bodyContainer = uiManager.createElement<UIElement>();

        _backPanel->nineSlice(uiManager.defaultPanelBackPanel());
        _backPanel->positionType(YGPositionTypeAbsolute);
        _backPanel->widthPercent(100);
        _backPanel->heightPercent(100);
        addChild(_backPanel);

        _bodyContainer->widthPercent(100);
        _bodyContainer->heightPercent(100);
        _bodyContainer->flexGrow(1.0f);
        _bodyContainer->flexShrink(1.0f);
        _bodyContainer->padding(8, 8, 8, 8);
        addChild(_bodyContainer);

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

    void GameUIInteractablesPanel::update(Engine &engine, sf::Time dt, sf::Vector2f parentOffset)
    {
        if (engine.currentSession())
        {
            auto &player = engine.currentSession()->playerController();
            if (player.controlling() == ControllingValue::ControlCharacter)
            {
                interactables(&player.canInteractWithInRange());
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
            _bodyContainer->removeChild(interactableUI);
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

        _bodyContainer->addChild(interactableUI);
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
                _bodyContainer->removeChild(interactableUI);
                _uiManager->removeElement(interactableUI);
                break;
            }
        }
    }
} // space