#include "game_ui_manager.hpp"

#include <stack>
#include <vector>
#include <algorithm>
#include <SFML/Window.hpp>

#include "ui_element.hpp"

#include "../utils.hpp"
#include "../engine.hpp"
#include "../keyboard.hpp"
#include "../game_session.hpp"
#include "../controllers/player_controller.hpp"

#include "./in_game_ui_page.hpp"

namespace space
{
    GameUIManager::GameUIManager(Engine &engine):
        _engine(engine),
        _defaultFont(nullptr)
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

        if (Utils::contains(_previousHoverPath, element))
        {
            sf::Event leaveEvent;
            leaveEvent.type = sf::Event::EventType::MouseLeft;
            element->trigger(leaveEvent);
        }

        element->parent(nullptr);

        return Utils::remove(_allElements, element);
    }

    void GameUIManager::processEvent(const sf::Event &event)
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

    void GameUIManager::updateUnderMouse(sf::Vector2f mousePosition)
    {
        std::stack<UIElement *> stack;
        stack.push(_bodyElement);

        _previousHoverPath = _currentHoverPath;
        _currentHoverPath.clear();

        while (!stack.empty())
        {
            auto current = stack.top();
            stack.pop();

            if (current->doesMouseHover(_engine, mousePosition))
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

    void GameUIManager::preUpdate(sf::Time dt)
    {
        _bodyElement->preUpdate(_engine, dt);
    }

    void GameUIManager::update(sf::Time dt)
    {
        auto renderSize = _engine.renderSize();
        auto bodyNode = _bodyElement->yogaNode();
        YGNodeCalculateLayout(bodyNode, renderSize.x, renderSize.y, YGDirectionLTR);

        _bodyElement->update(_engine, dt, sf::Vector2f());

        auto mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*_engine.window()));
        mousePosition /= _engine.cameraScale();
        updateUnderMouse(mousePosition);
    }

    void GameUIManager::draw(RenderCamera &target)
    {
        auto size = static_cast<sf::Vector2f>(_engine.renderSize());
        sf::View view(size * 0.5f, size);
        target.texture().setView(view);

        _bodyElement->draw(_engine, target);
    }

    void GameUIManager::initDefaultWindows()
    {
        _inGameUIPage = createElement<InGameUIPage>();
        body()->addChild(_inGameUIPage);
    }
} // space