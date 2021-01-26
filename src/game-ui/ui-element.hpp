#pragma once

#include <SFML/Graphics.hpp>
#include <yoga/Yoga.h>

#include <vector>
#include <map>
#include <functional>

#include "../space_transform.hpp"

namespace space
{
    class Engine;
    class RenderCamera;

    class UIElement
    {
        public:
            // Fields

            // Constructor
            UIElement() : _parent(nullptr), _yogaNode(YGNodeNew()) { }
            virtual ~UIElement() { }

            // Methods
            virtual void update(Engine &engine, sf::Time dt);
            virtual void draw(Engine &engine, RenderCamera &target) = 0;
            virtual bool doesMouseHover(Engine &engine, sf::Vector2f mousePosition) const { return false; }

            const std::vector<UIElement *> &children() const { return _children; }

            void addChild(UIElement *element);
            void removeChild(UIElement *element);

            UIElement *parent() { return _parent; }
            void parent(UIElement *parent);

            typedef std::function<bool (const sf::Event &)> EventHandler;

            YGNodeRef yogaNode() { return _yogaNode; }

        protected:
            // Fields
            YGNodeRef _yogaNode;
            std::vector<UIElement *> _children;
            UIElement *_parent;
            sf::Transform _transform;
            std::map<std::string, std::vector<EventHandler>> _eventHandlers;
    };
} // space