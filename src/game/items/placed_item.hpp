#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "../space_object.hpp"
#include "item.hpp"

#include "../interactions/interactable.hpp"

namespace space
{
    class PlaceableItem;
    class GameSession;
    class WalkableArea;
    class DrawLayer;

    class PlacedItem : public SpaceObject
    {
        public:
            // Fields
            PlaceableItem *item;
            WalkableArea &area;
            DrawLayer &onLayer;

            // Constructor
            PlacedItem(PlaceableItem *item, const sf::Vector2f &position, WalkableArea &area, DrawLayer &onLayer);
            virtual ~PlacedItem();

            // Methods
            static const std::string SpaceObjectType() { return "placed-item"; }
            virtual std::string type() const { return SpaceObjectType(); }

            void addPhysics(b2World &world);
            void removePhysics(b2World &world);

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, sf::RenderTarget &target);

        private:
            // Fields
            sf::Sprite _sprite;
            b2Body *_collider;

            // Methods
    };

    template <typename T>
    class PlacedItemPair
    {
        public:
            // Fields
            const PlacedItem *placed;
            const T *item;

            // Constructor
            PlacedItemPair<T>(PlacedItem *placed, T *item) : placed(placed), item(item) { }

            // Methods
    };
}