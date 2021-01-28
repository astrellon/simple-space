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

    enum class ElementType
    {
        Unknown, Root, Text, Image
    };

    class UIElement
    {
        public:
            // Types
            typedef std::function<bool (const sf::Event &)> EventHandler;

            // Fields

            // Constructor
            UIElement() : _parent(nullptr), _yogaNode(YGNodeNew()) { }
            virtual ~UIElement() { }

            // Methods
            virtual void update(Engine &engine, sf::Time dt);
            virtual void draw(Engine &engine, RenderCamera &target);
            virtual bool doesMouseHover(Engine &engine, sf::Vector2f mousePosition) const { return false; }

            const std::vector<UIElement *> &children() const { return _children; }

            void addChild(UIElement *element);
            void removeChild(UIElement *element);

            UIElement *parent() { return _parent; }
            void parent(UIElement *parent);

            YGNodeRef yogaNode() { return _yogaNode; }

            virtual ElementType elementType() const = 0;

            void padding(YGEdge edge, float padding) { YGNodeStyleSetPadding(_yogaNode, edge, padding); }
            void paddingPercent(YGEdge edge, float padding) { YGNodeStyleSetPaddingPercent(_yogaNode, edge, padding); }
            YGValue padding(YGEdge edge) const { return YGNodeStyleGetPadding(_yogaNode, edge); }

            void margin(YGEdge edge, float margin) { YGNodeStyleSetPadding(_yogaNode, edge, margin); }
            void marginPercent(YGEdge edge, float margin) { YGNodeStyleSetPaddingPercent(_yogaNode, edge, margin); }
            YGValue margin(YGEdge edge) const { return YGNodeStyleGetPadding(_yogaNode, edge); }

            float layoutLeft() const { return YGNodeLayoutGetLeft(_yogaNode); }
            float layoutRight() const { return YGNodeLayoutGetRight(_yogaNode); }
            float layoutTop() const { return YGNodeLayoutGetTop(_yogaNode); }
            float layoutBottom() const { return YGNodeLayoutGetBottom(_yogaNode); }
            float layoutWidth() const { return YGNodeLayoutGetWidth(_yogaNode); }
            float layoutHeight() const { return YGNodeLayoutGetHeight(_yogaNode); }

        protected:
            // Fields
            YGNodeRef _yogaNode;
            std::vector<UIElement *> _children;
            UIElement *_parent;
            sf::Transform _transform;
            std::map<std::string, std::vector<EventHandler>> _eventHandlers;

            // Methods
            virtual void drawChildren(Engine &engine, RenderCamera &target);
            virtual void drawSelf(Engine &engine, RenderCamera &target) { }
    };

    class UIRootElement : public UIElement
    {
        public:
            // Fields

            // Constructor

            // Methods
            virtual ElementType elementType() const { return ElementType::Root; }
    };

} // space