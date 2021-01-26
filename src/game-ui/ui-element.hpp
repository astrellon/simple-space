#pragma once

#include <SFML/Graphics.hpp>
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
            UIElement() : _parent(nullptr) { }
            virtual ~UIElement() { }

            // Methods
            virtual void update(Engine &engine, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(Engine &engine, RenderCamera &target) = 0;
            virtual bool doesMouseHover(Engine &engine, sf::Vector2f mousePosition) const { return false; }

            const std::vector<UIElement *> &children() const { return _children; }

            void addChild(UIElement *element);
            void removeChild(UIElement *element);

            UIElement *parent() { return _parent; }
            void parent(UIElement *parent);

            const SpaceTransform &transform() const { return _transform; }
            SpaceTransform &transform() { return _transform; }
            void transform(const SpaceTransform &transform) { _transform = transform; }
            void transform(const SpaceTransform &&transform) { _transform = transform; }
            const sf::Transform &worldTransform() const { return _worldTransform; }

            typedef std::function<bool (const sf::Event &)> EventHandler;

        protected:
            // Fields
            std::vector<UIElement *> _children;
            UIElement *_parent;
            SpaceTransform _transform;
            sf::Transform _worldTransform;
            std::map<std::string, std::vector<EventHandler>> _eventHandlers;

            // Methods
            inline void updateWorldTransform(const sf::Transform &parentTransform)
            {
                _worldTransform = parentTransform * _transform.getTransform();
            }
    };
} // space