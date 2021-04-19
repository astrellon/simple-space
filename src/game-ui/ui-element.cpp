#include "ui-element.hpp"

#include <SFML/Graphics.hpp>

#include "../utils.hpp"
#include "../debug/draw_debug.hpp"
#include "../render_camera.hpp"

namespace space
{
    void UIElement::update(Engine &engine, sf::Time dt, sf::Vector2f parentOffset)
    {
        auto left = YGNodeLayoutGetLeft(_yogaNode);
        auto top = YGNodeLayoutGetTop(_yogaNode);

        sf::Vector2f offset(std::round(left) + parentOffset.x, std::round(top) + parentOffset.y);

        Utils::setPosition(offset, _transform);

        for (auto child : _children)
        {
            child->update(engine, dt, offset);
        }
    }

    void UIElement::draw(Engine &engine, RenderCamera &target)
    {
        drawSelf(engine, target);
        drawChildren(engine, target);

        if (DrawDebug::highlightElement == this)
        {
            drawOutline(engine, target);
        }
    }

    void UIElement::addChild(UIElement *element)
    {
        if (element == nullptr || Utils::contains(_children, element))
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
        if (element == nullptr || !Utils::contains(_children, element))
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

    void UIElement::drawChildren(Engine &engine, RenderCamera &target)
    {
        for (auto child : _children)
        {
            child->draw(engine, target);
        }
    }

    void UIElement::drawOutline(Engine &engine, RenderCamera &target)
    {
        sf::RectangleShape shape;

        auto width = YGNodeLayoutGetWidth(_yogaNode);
        auto height = YGNodeLayoutGetHeight(_yogaNode);
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(Utils::getPosition(_transform));
        shape.setFillColor(sf::Color(79, 164, 184, 100));
        shape.setOutlineColor(sf::Color(146, 232, 192, 100));
        shape.setOutlineThickness(1.0f);

        target.texture().draw(shape);
    }
} // space