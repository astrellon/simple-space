#include "ui-element.hpp"

#include "../utils.hpp"

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
} // space