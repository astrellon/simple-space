#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

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
            static const SpaceObjectType TypeValue;

            PlaceableItem *item;

            // Constructor
            PlacedItem(const ItemId &itemId);
            PlacedItem(PlaceableItem *item);
            PlacedItem(const ObjectId &id, PlaceableItem *item);
            PlacedItem(const ObjectId &id, const ItemId &itemId);
            virtual ~PlacedItem();

            // Methods
            virtual SpaceObjectType type() const { return TypeValue; }

            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) { return clonePlacedItem(newId, context); }
            PlacedItem *clonePlacedItem(const ObjectId &newId, const CloneContext &context);

            virtual void insideArea(Area *area);
            virtual Area *insideArea() const { return _insideArea; }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual void onPostLoad(GameSession &session, LoadingContext &context);
            virtual bool doesMouseHover(const Area *inRelationTo, GameSession &session, sf::Vector2f mousePosition) const;

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