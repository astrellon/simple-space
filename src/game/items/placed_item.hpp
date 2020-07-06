#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "../space_object.hpp"
#include "item.hpp"

namespace space
{
    class PlaceableItem;
    class GameSession;
    class WalkableArea;

    class PlacedItem : public SpaceObject
    {
        public:
            // Fields
            PlaceableItem *item;
            WalkableArea &area;

            // Constructor
            PlacedItem(PlaceableItem *item, const sf::Vector2f &position, WalkableArea &area);
            virtual ~PlacedItem();

            // Methods
            void addPhysics(b2World &world);
            void removePhysics(b2World &world);

            float interactRadiusSquared() const { return _interactRadiusSquared; }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, sf::RenderTarget &target);

        private:
            // Fields
            sf::Sprite _sprite;
            b2Body *_triggerSensor;
            float _interactRadiusSquared;

            // Methods
    };
}