#include "game-ui-interactables-window.hpp"

#include "./game-ui-manager.hpp"
#include "./ui-text-element.hpp"
#include "./ui-button.hpp"
#include "./game-ui-interactable.hpp"

#include "../game/interactions/interactable.hpp"
#include "../game/interactions/interactable_list.hpp"

namespace space
{
    void GameUIInteractablesWindow::init(GameUIManager &uiManager)
    {
        GameUIWindow::init(uiManager);
        _uiManager = &uiManager;

        width(200);
        heightAuto();
        flexShrink(1.0f);
        margin(16, 16, 16, 16);

        header()->text("Interactables");
    }

    void GameUIInteractablesWindow::interactables(const InteractableList *interactables)
    {
        _removeOnAddInteractable.reset();
        _removeOnRemoveInteractable.reset();

        _interactables = interactables;

        if (interactables)
        {
            for (auto interactable : interactables->interactables())
            {
                addInteractable(interactable);
            }

            interactables->onAddInteractable.connect([this](Interactable *interactable)
            {
                std::cout << "Added interactable" << std::endl;
                this->addInteractable(interactable);
            });
            interactables->onRemoveInteractable.connect([this](Interactable *interactable)
            {
                std::cout << "Removed interactable" << std::endl;
                this->removeInteractable(interactable);
            });
        }
    }

    void GameUIInteractablesWindow::addInteractable(Interactable *interactable)
    {
        auto interactableUI = _uiManager->createElement<GameUIInteractable>();
        interactableUI->interactable(interactable);

        bodyContainer()->addChild(interactableUI);
        _interactableUIs.push_back(interactableUI);
    }

    void GameUIInteractablesWindow::removeInteractable(Interactable *interactable)
    {
        for (auto iter = _interactableUIs.begin(); iter != _interactableUIs.end(); ++iter)
        {
            auto interactableUI = *iter;
            if (interactableUI->interactable() == interactable)
            {
                _interactableUIs.erase(iter);
                bodyContainer()->removeChild(interactableUI);
                _uiManager->removeElement(interactableUI);
                break;
            }
        }
    }
} // space