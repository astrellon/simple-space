#include "game-ui-manager.hpp"

#include "ui-element.hpp"

#include "../utils.hpp"
#include "../engine.hpp"

namespace space
{
    GameUIManager::GameUIManager(): _rootElement(nullptr), _currentHover(nullptr), _body(YGNodeNew())
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

        auto renderSize = engine.renderSize();
        YGNodeStyleSetWidth(_body, renderSize.x);
        YGNodeStyleSetHeight(_body, renderSize.y);
        YGNodeCalculateLayout(_body, renderSize.x, renderSize.y, YGDirectionLTR);

        _rootElement->update(engine, dt);
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

    void GameUIManager::rootElement(UIElement *element)
    {
        if (_rootElement && _rootElement->yogaNode())
        {
            YGNodeRemoveChild(_body, _rootElement->yogaNode());
        }

        _rootElement = element;

        if (element && element->yogaNode())
        {
            YGNodeInsertChild(_body, element->yogaNode(), 0);
        }
    }

} // space