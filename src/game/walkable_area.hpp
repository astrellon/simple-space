#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

namespace space
{
    class PlaceableItem;
    class Character;
    class Engine;

    class PlacedItem
    {
        public:
            // Fields
            PlaceableItem const & item;
            const sf::Vector2f position;

            // Constructor
            PlacedItem(PlaceableItem const &item, const sf::Vector2f &position);

            // Methods
            void draw(sf::RenderTarget &target, const sf::Transform &parentTransform);

        private:
            // Fields
            sf::Sprite _sprite;

            // Methods
    };

    class WalkableArea
    {
        public:
            // Fields

            // Constructor
            WalkableArea();
            ~WalkableArea();

            // Methods
            void update(Engine &engine, sf::Time dt, const sf::Transform &parentTransform);
            void draw(Engine &engine, sf::RenderTarget &target);

            void addCharacter(Character *character);
            void removeCharacter(Character *character);

            void addPlaceable(PlaceableItem *item, sf::Vector2f position);
            void removePlaceable(PlaceableItem *item);

        private:
            // Fields
            std::vector<Character *> _characters;
            std::vector<PlacedItem> _placedItems;
            sf::Transform _worldTransform;

            b2World _physicsWorld;

            // Methods
    };
} // space