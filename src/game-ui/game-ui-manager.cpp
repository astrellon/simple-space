#include "game-ui-manager.hpp"

#include "ui-element.hpp"

#include "../utils.hpp"
#include "../engine.hpp"

namespace space
{
    GameUIManager::GameUIManager(): _rootElement(nullptr), _currentHover(nullptr)
    {

    }

    bool GameUIManager::removeElement(UIElement *element)
    {
        if (element == nullptr)
        {
            return false;
        }

        element->parent(nullptr);

        return Utils::remove(_allElements, element);
    }

    void GameUIManager::processEvent(Engine &engine, const sf::Event &event)
    {

    }

    void GameUIManager::update(Engine &engine, sf::Time dt)
    {
        if (!_rootElement)
        {
            return;
        }

        _rootElement->update(engine, dt, sf::Transform::Identity);
    }

    void GameUIManager::draw(Engine &engine, RenderCamera &target)
    {
        if (!_rootElement)
        {
            return;
        }

        auto view = engine.window()->getView();
        target.texture().setView(view);

        _rootElement->draw(engine, target);
    }
} // space