#include "ui-element.hpp"

#include "../utils.hpp"

namespace space
{
    void UIElement::update(Engine &engine, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);

        for (auto child : _children)
        {
            child->update(engine, dt, _worldTransform);
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
    }

    void UIElement::removeChild(UIElement *element)
    {
        if (element == nullptr || !Utils::contains(_children, element))
        {
            return;
        }

        Utils::remove(_children, element);
        element->parent(nullptr);
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

} // space