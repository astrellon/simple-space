#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "../space_object.hpp"
#include "item.hpp"

namespace space
{
    class PlaceableItem;
    class Character;
    class GameSession;
    class Ship;
    class PolygonCollider;

    class PlacedItem : public SpaceObject
    {
        public:
            // Fields
            PlaceableItem const *item;

            // Constructor
            PlacedItem(PlaceableItem const *item, const sf::Vector2f &position);
            virtual ~PlacedItem();

            // Methods
            void addPhysics(b2World &world);
            void removePhysics(b2World &world);

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, sf::RenderTarget &target);

        private:
            // Fields
            sf::Sprite _sprite;
            b2Body *_triggerSensor;

            // Methods
    };
}