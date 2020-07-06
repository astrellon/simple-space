#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "items/item.hpp"
#include "items/placed_item.hpp"

namespace space
{
    class PlaceableItem;
    class Character;
    class GameSession;
    class Ship;
    class PolygonCollider;

    class WalkableArea
    {
        public:
            // Fields

            // Constructor
            WalkableArea();
            ~WalkableArea();

            // Methods
            void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            void draw(GameSession &session, sf::RenderTarget &target);

            void partOfShip(Ship *ship) { _partOfShip = ship; }
            Ship *partOfShip() const { return _partOfShip; }

            void addStaticCollider(PolygonCollider &collider);
            void removeStaticCollider(PolygonCollider &collider);

            void addCharacter(Character *character);
            void removeCharacter(Character *character);

            void addPlaceable(PlaceableItem *item, sf::Vector2f position);
            void removePlaceable(ItemId id);

            b2World &physicsWorld() { return _physicsWorld; }

        private:
            // Fields
            std::vector<Character *> _characters;
            std::vector<std::unique_ptr<PlacedItem>> _placedItems;
            sf::Transform _worldTransform;
            Ship *_partOfShip;
            GameSession *_session;

            b2World _physicsWorld;

            // Methods
            void checkForInteractables(GameSession &session);
    };
} // space