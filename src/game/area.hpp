#pragma once

#include <vector>
#include <memory>
#include <functional>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "draw_layer.hpp"
#include "draw_layers.hpp"
#include "../non_copyable.hpp"

namespace space
{
    class SpaceObject;
    class PolygonCollider;
    class GameSession;
    class LoadingContext;
    class RenderCamera;
    class PlanetSurface;
    class Ship;
    class StarSystem;
    class PlacedItem;
    class PlaceableItem;

    template <typename T>
    class PlacedItemPair;
    class Teleporter;

    enum class AreaType
    {
        StarSystem, Ship, PlanetSurface
    };

    class Area : private NonCopyable
    {
        public:

            typedef std::function<void(SpaceObject *)> FindObjectCallback;

            // Fields

            // Constructor
            Area(AreaType type, SpaceObject *partOfObject);
            ~Area();

            // Methods
            AreaType type() const { return _type; }

            void addStaticCollider(PolygonCollider &collider);
            void removeStaticCollider(PolygonCollider &collider);

            void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            void draw(GameSession &session, RenderCamera &target);
            void onPostLoad(GameSession &session, LoadingContext &context);
            bool checkForMouse(GameSession &session, sf::Vector2f mousePosition) const;

            void addObject(SpaceObject *obj);
            void removeObject(SpaceObject *obj);
            const std::vector<SpaceObject *> &objects() const { return _objects; }

            PlacedItem *addPlaceable(GameSession &session, PlaceableItem *item, sf::Vector2f position);

            std::vector<PlacedItemPair<Teleporter>> findTeleporters() const;
            void addTeleporters(std::vector<PlacedItemPair<Teleporter>> &result) const;

            void getObjectsNearby(float radius, const sf::Vector2f &position, FindObjectCallback callback) const;
            void getObjectsNearby(float radius, const sf::Vector2f &position, std::vector<SpaceObject *> &result) const;

            SpaceObject *partOfObject() const { return _partOfObject; }
            PlanetSurface *partOfPlanetSurface() const { return _type == AreaType::PlanetSurface ? _partOfPlanetSurface : nullptr; }
            Ship *partOfShip() const { return _type == AreaType::Ship ? _partOfShip : nullptr; }
            StarSystem *partOfStarSystem() const { return _type == AreaType::StarSystem ? _partOfStarSystem : nullptr; }

            b2World *physicsWorld() const { return _physicsWorld.get(); }

        private:
            // Fields
            AreaType _type;
            std::vector<SpaceObject *> _objects;
            std::unique_ptr<b2World> _physicsWorld;
            SpaceObject *_partOfObject;
            sf::Transform _transform;

            union
            {
                Ship *_partOfShip;
                PlanetSurface *_partOfPlanetSurface;
                StarSystem *_partOfStarSystem;
            };

            DrawLayer _background;
            DrawLayer _main;
            DrawLayer _foreground;

            // Methods
            bool tryGetLayer(DrawLayers::Type layer, DrawLayer **result);
    };
} // space