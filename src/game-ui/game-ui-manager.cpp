#include "game-ui-manager.hpp"

#include "ui-element.hpp"

#include "../utils.hpp"
#include "../engine.hpp"

namespace space
{
    GameUIManager::GameUIManager(): _currentHover(nullptr)
    {
        _bodyElement = createElement<UIRootElement>();
    }

    bool GameUIManager::removeElement(UIElement *element)
    {
        if (element == nullptr)
        {
            return false;
        }

        if (element == _bodyElement)
        {
            std::cout << "Cannot remove body element" << std::endl;
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
        auto renderSize = engine.renderSize();
        auto bodyNode = _bodyElement->yogaNode();
        YGNodeStyleSetWidth(bodyNode, renderSize.x);
        YGNodeStyleSetHeight(bodyNode, renderSize.y);
        YGNodeCalculateLayout(bodyNode, renderSize.x, renderSize.y, YGDirectionLTR);

        _bodyElement->update(engine, dt);
    }

    void GameUIManager::draw(Engine &engine, RenderCamera &target)
    {
        auto view = engine.window()->getView();
        target.texture().setView(view);

        _bodyElement->draw(engine, target);
    }
} // space