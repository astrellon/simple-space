#include "game-ui-manager.hpp"

#include <stack>
#include <vector>
#include <algorithm>

#include "ui-element.hpp"

#include "../utils.hpp"
#include "../engine.hpp"

namespace space
{
    GameUIManager::GameUIManager(): _defaultFont(nullptr)
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
        for (int i = _currentHoverPath.size() - 1; i >= 0; --i)
        {
            auto result = _currentHoverPath[i]->trigger(event);
            if (result == UIEventResult::Captured)
            {
                break;
            }
        }

    }

    void GameUIManager::updateUnderMouse(Engine &engine, sf::Vector2f mousePosition)
    {
        std::stack<UIElement *> stack;
        stack.push(_bodyElement);

        _previousHoverPath = _currentHoverPath;
        _currentHoverPath.clear();

        while (!stack.empty())
        {
            auto current = stack.top();
            stack.pop();

            if (current->doesMouseHover(engine, mousePosition))
            {
                _currentHoverPath.push_back(current);
                auto &children = current->children();
                for (auto iter = children.rbegin(); iter != children.rend(); ++iter)
                {
                    stack.push(*iter);
                }
            }
        }

        sf::Event enteredEvent;
        enteredEvent.type = sf::Event::EventType::MouseEntered;

        for (auto element : _currentHoverPath)
        {
            if (!Utils::contains(_previousHoverPath, element))
            {
                element->trigger(enteredEvent);
            }
        }

        sf::Event leaveEvent;
        leaveEvent.type = sf::Event::EventType::MouseLeft;

        for (auto element : _previousHoverPath)
        {
            if (!Utils::contains(_currentHoverPath, element))
            {
                element->trigger(leaveEvent);
            }
        }
    }

    void GameUIManager::update(Engine &engine, sf::Time dt)
    {
        auto renderSize = engine.renderSize();
        auto bodyNode = _bodyElement->yogaNode();
        YGNodeCalculateLayout(bodyNode, renderSize.x, renderSize.y, YGDirectionLTR);

        _bodyElement->update(engine, dt, sf::Vector2f());

        auto mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*engine.window()));
        mousePosition /= engine.cameraScale();
        updateUnderMouse(engine, mousePosition);
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