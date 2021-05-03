#include "game-ui-manager.hpp"

#include <stack>
#include <vector>
#include <algorithm>

#include "ui-element.hpp"

#include "../utils.hpp"
#include "../engine.hpp"

namespace space
{
    GameUIManager::GameUIManager(): _currentHover(nullptr), _defaultFont(nullptr)
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
        if (event.type == sf::Event::MouseMoved)
        {
            auto &sceneRender = engine.sceneRender();
            auto mousePosition = sf::Mouse::getPosition(*engine.window());
            auto worldMousePosition = engine.window()->mapPixelToCoords(mousePosition, sceneRender.camera().view());

            auto overPath = findElementUnderMouse(engine, worldMousePosition);

            std::cout << "Mouse Moved: ";
            for (auto over : overPath)
            {
                std::cout << Utils::elementTypeName(over->elementType()) << " | ";
            }
            std::cout << std::endl;
        }
    }

    std::vector<UIElement *> GameUIManager::findElementUnderMouse(Engine &engine, sf::Vector2f worldMousePosition) const
    {
        std::stack<UIElement *> stack;
        stack.push(_bodyElement);

        std::vector<UIElement *> overPath;

        while (!stack.empty())
        {
            auto current = stack.top();
            stack.pop();

            if (current->doesMouseHover(engine, worldMousePosition))
            {
                overPath.push_back(current);
                for (auto child : current->children())
                {
                    stack.push(child);
                }
            }
        }

        return overPath;
    }

    void GameUIManager::update(Engine &engine, sf::Time dt)
    {
        auto renderSize = engine.renderSize();
        auto bodyNode = _bodyElement->yogaNode();
        YGNodeCalculateLayout(bodyNode, renderSize.x, renderSize.y, YGDirectionLTR);

        _bodyElement->update(engine, dt, sf::Vector2f());
    }

    void GameUIManager::draw(Engine &engine, RenderCamera &target)
    {
        auto size = static_cast<sf::Vector2f>(engine.renderSize());
        sf::View view(size * 0.5f, size);
        target.texture().setView(view);

        _bodyElement->draw(engine, target);
    }

            void checkForMouse(Engine &engine, sf::Vector2f worldMousePosition);
} // space