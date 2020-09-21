#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "items/item.hpp"
#include "items/placed_item.hpp"
#include "draw_layer.hpp"
#include "draw_layers.hpp"

#include "../non_copyable.hpp"

namespace space
{
    class PlaceableItem;
    class Character;
    class GameSession;
    class Ship;
    class PolygonCollider;
    class PlanetSurface;
    class GrassEffect;

    template <typename T>
    class PlacedItemPair;
    class Teleporter;

    class WalkableArea : private NonCopyable
    {
        private:
            enum class PostLoadType
            {
                Character, GrassEffect, Item
            };

            struct PostLoadObject
            {
                ObjectId id;
                ItemId itemId;
                PostLoadType type;
                sf::Vector2f position;

                PostLoadObject(const ObjectId &id, PostLoadType type) : id(id), type(type) { }
                PostLoadObject(ItemId itemId, sf::Vector2f position) : itemId(itemId), type(PostLoadType::Item), position(position) { }
            };

        public:

            // Fields

            // Constructor
            WalkableArea();
            ~WalkableArea();

            // Methods
            void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            void draw(GameSession &session, sf::RenderTarget &target);
            void onPostLoad(GameSession &session);

            std::vector<PlacedItemPair<Teleporter>> findTeleporters() const;

            void partOfShip(Ship *ship) { _partOfShip = ship; }
            Ship *partOfShip() const { return _partOfShip; }

            void partOfPlanetSurface(PlanetSurface *planetSurface) { _partOfPlanet = planetSurface; }
            PlanetSurface *partOfPlanetSurface() const { return _partOfPlanet; }

            void addStaticCollider(PolygonCollider &collider);
            void removeStaticCollider(PolygonCollider &collider);

            void addPostLoadCharacter(const ObjectId &id) { _onPostLoadObjects.emplace_back(id, PostLoadType::Character); }
            void addPostLoadGrassEffect(const ObjectId &id) { _onPostLoadObjects.emplace_back(id, PostLoadType::GrassEffect); }
            void addPostLoadPlaceable(ItemId id, sf::Vector2f position) { _onPostLoadObjects.emplace_back(id, position); }

            void addCharacter(Character *character);
            void removeCharacter(Character *character);
            const std::vector<Character *> &characters() const { return _characters; }

            void addGrassEffect(GrassEffect *grassEffect);
            void removeGrassEffect(const ObjectId &id);
            const std::vector<GrassEffect *> &grassEffects() const { return _grassEffects; }

            PlacedItem *addPlaceable(PlaceableItem *item, sf::Vector2f position);
            void removePlaceable(ItemId id);
            const std::vector<std::unique_ptr<PlacedItem>> &placedItems() const { return _placedItems; }

            b2World &physicsWorld() { return _physicsWorld; }

        private:
            // Fields
            std::vector<PostLoadObject> _onPostLoadObjects;

            std::vector<Character *> _characters;
            std::vector<std::unique_ptr<PlacedItem>> _placedItems;
            std::vector<GrassEffect *> _grassEffects;
            sf::Transform _worldTransform;
            Ship *_partOfShip;
            PlanetSurface *_partOfPlanet;
            GameSession *_session;

            b2World _physicsWorld;

            DrawLayer _background;
            DrawLayer _main;
            DrawLayer _foreground;

            // Methods
            bool tryGetLayer(DrawLayers::Type layerType, DrawLayer **result);
    };
} // space