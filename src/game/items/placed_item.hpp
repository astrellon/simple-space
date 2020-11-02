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
    class LoadingContext;
    class RenderCamera;

    class PlacedItem : public SpaceObject
    {
        public:
            // Fields
            PlaceableItem *item;

            // Constructor
            PlacedItem(const ItemId &itemId);
            PlacedItem(PlaceableItem *item);
            PlacedItem(const ObjectId &id, PlaceableItem *item);
            PlacedItem(const ObjectId &id, const ItemId &itemId);
            virtual ~PlacedItem();

            // Methods
            virtual SpaceObject *clone(const ObjectId &newId, GameSession &session) { return clonePlacedItem(newId, session); }
            PlacedItem *clonePlacedItem(const ObjectId &newId, GameSession &session);

            static const std::string SpaceObjectType() { return "placed-item"; }
            virtual std::string type() const { return SpaceObjectType(); }

            virtual void insideArea(Area *area);
            virtual Area *insideArea() const { return _insideArea; }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual void onPostLoad(GameSession &session, LoadingContext &context);
            virtual bool doesMouseHover(GameSession &session, sf::Vector2f mousePosition) const;
            virtual bool isGenerated() const { return true; }

            virtual DrawLayers::Type drawLayer() const;

        private:
            // Fields
            ItemId _itemId;
            sf::Sprite _sprite;
            b2Body *_collider;
            sf::FloatRect _spriteBounds;

            // Methods
            void processItem();
            void addPhysics(b2World &world);
            void removePhysics(b2World &world);
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