#include "ui_element.hpp"

#include <algorithm>
#include <SFML/Graphics.hpp>

#include "../utils.hpp"
#include "../debug/draw_debug.hpp"
#include "../render_camera.hpp"

namespace space
{
    int UIElement::_nextHandlerId = 0;

    UIElement::~UIElement()
    {
        _destroyed = true;
        for (auto remove : _removeHandlers)
        {
            remove();
        }
        _removeHandlers.clear();
    }

    void UIElement::preUpdate(Engine &engine, sf::Time dt)
    {
        if (_destroyed)
        {
            return;
        }

        for (auto child : _children)
        {
            child->preUpdate(engine, dt);
        }
    }

    void UIElement::update(Engine &engine, sf::Time dt, sf::Vector2f parentOffset)
    {
        if (_destroyed || !_visible)
        {
            return;
        }

        auto left = YGNodeLayoutGetLeft(_yogaNode);
        auto top = YGNodeLayoutGetTop(_yogaNode);

        sf::Vector2f offset(std::round(left) + parentOffset.x, std::round(top) + parentOffset.y);

        Utils::setPosition(offset, _transform);

        for (auto child : _children)
        {
            if (!child->visible())
            {
                continue;
            }

            child->update(engine, dt, offset);
        }
    }

    void UIElement::draw(Engine &engine, RenderCamera &target)
    {
        if (_destroyed || !_visible)
        {
            return;
        }

        drawSelf(engine, target);
        drawChildren(engine, target);

        if (DrawDebug::highlightElement == this)
        {
            drawOutline(engine, target);
        }
    }

    bool UIElement::doesMouseHover(Engine &engine, sf::Vector2f mousePosition) const
    {
        if (_destroyed || !_visible)
        {
            return false;
        }

        auto worldPos = Utils::getPosition(_transform);
        auto local = mousePosition - worldPos;
        auto size = getSize();
        sf::FloatRect bounds(sf::Vector2f(), size);
        return bounds.contains(local);
    }

    void UIElement::addChild(UIElement *element)
    {
        if (_destroyed || element == nullptr || Utils::contains(_children, element))
        {
            return;
        }

        _children.push_back(element);
        element->parent(this);
        if (element->yogaNode() && _yogaNode)
        {
            auto endIndex = YGNodeGetChildCount(_yogaNode);
            YGNodeInsertChild(_yogaNode, element->yogaNode(), endIndex);
        }
    }

    void UIElement::removeChild(UIElement *element)
    {
        if (_destroyed || element == nullptr || !Utils::contains(_children, element))
        {
            return;
        }

        Utils::remove(_children, element);
        element->parent(nullptr);

        if (element->yogaNode() && _yogaNode)
        {
            YGNodeRemoveChild(_yogaNode, element->yogaNode());
        }
    }

    void UIElement::parent(UIElement *parent)
    {
        if (_parent)
        {
            _parent->removeChild(this);
        }

        _parent = parent;

        if (_parent)
        {
            _parent->addChild(this);
        }
    }

    UIElement::RemoveEventHandler UIElement::on(sf::Event::EventType type, UIElement::EventHandler handler)
    {
        if (_destroyed)
        {
            return [](){};
        }

        auto &list = _eventHandlers[type];
        auto id = nextHandlerId();
        list.emplace_back(id, handler);

        auto result = [&list, id]() mutable
        {
            for (auto iter = list.begin(); iter != list.end(); ++iter)
            {
                if (iter->first == id)
                {
                    list.erase(iter);
                    return;
                }
            }

            std::cout << "Unable to find event listener to remove." << std::endl;
        };

        _removeHandlers.push_back(result);

        return result;
    }

    UIElement::RemoveEventHandler UIElement::onClick(UIElement::EventHandler handler)
    {
        return on(sf::Event::EventType::MouseButtonPressed, handler);
    }

    UIEventResult UIElement::trigger(const sf::Event &event)
    {
        if (_destroyed)
        {
            return UIEventResult::NotFound;
        }

        auto find = _eventHandlers.find(event.type);
        if (find == _eventHandlers.end())
        {
            return UIEventResult::NotFound;
        }

        auto list = find->second;
        for (auto handler : list)
        {
            auto result = handler.second(event);
            if (result == UIEventResult::Captured)
            {
                return result;
            }
        }

        return UIEventResult::Triggered;
    }

    void UIElement::visible(bool value)
    {
        if (value == _visible)
        {
            return;
        }

        _visible = value;

        if (value)
        {
            YGNodeStyleSetPositionType(_yogaNode, _position);
        }
        else
        {
            YGNodeStyleSetPositionType(_yogaNode, YGPositionTypeAbsolute);
        }
    }

    void UIElement::drawChildren(Engine &engine, RenderCamera &target)
    {
        for (auto child : _children)
        {
            if (!child->visible())
            {
                continue;
            }

            child->draw(engine, target);
        }
    }

    void UIElement::drawOutline(Engine &engine, RenderCamera &target)
    {
        sf::RectangleShape shape;

        auto size = getSize();
        shape.setSize(size);
        shape.setPosition(Utils::getPosition(_transform));
        shape.setFillColor(sf::Color(79, 164, 184, 100));
        shape.setOutlineColor(sf::Color(146, 232, 192, 100));
        shape.setOutlineThickness(1.0f);

        target.texture().draw(shape);
    }
} // space