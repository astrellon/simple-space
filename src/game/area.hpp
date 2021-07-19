#pragma once

#include <vector>
#include <memory>
#include <functional>

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include "draw_layer.hpp"
#include "draw_layers.hpp"
#include "../non_copyable.hpp"
#include "../types.hpp"
#include "clone_context.hpp"
#include "space_object_type.hpp"
#include "area_quadtree.hpp"

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
    class SpacePortal;

    template <typename T>
    class PlacedItemPair;
    class Teleporter;

    class TeleporterList;

    enum class AreaType
    {
        StarSystem, Ship, PlanetSurface
    };

    class Area : private NonCopyable
    {
        public:
            // Fields

            // Constructor
            Area(AreaType type, SpaceObject *partOfObject);
            ~Area();

            // Methods
            AreaType type() const { return _type; }

            void cloneInto(const ObjectId &newIdPrefix, Area &targetArea, const CloneContext &context) const;

            void addStaticCollider(PolygonCollider &collider);
            void removeStaticCollider(PolygonCollider &collider);

            void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            void draw(GameSession &session, RenderCamera &target);
            void onPostLoad(GameSession &session, LoadingContext &context);
            bool checkForMouse(const Area *inRelationTo, GameSession &session, sf::Vector2f mousePosition) const;

            void addObject(SpaceObject *obj);
            void removeObject(SpaceObject *obj);
            const std::vector<SpaceObject *> &objects() const { return _objects; }
            const std::vector<SpaceObject *> &objects(SpaceObjectType type) const;

            PlacedItem *addPlaceable(GameSession &session, PlaceableItem *item, sf::Vector2f position);

            void addTeleporters(TeleporterList &result) const;

            void getObjectsNearby(sf::FloatRect inRect, std::vector<SpaceObject *> &result) const;
            void getObjectsNearby(float radius, const SpaceObject &toObjects, std::vector<SpaceObject *> &result) const;
            void getObjectsNearby(float radius, const sf::Vector2f &position, std::vector<SpaceObject *> &result) const;

            SpaceObject *partOfObject() const { return _partOfObject; }
            PlanetSurface *partOfPlanetSurface() const { return _type == AreaType::PlanetSurface ? _partOfPlanetSurface : nullptr; }
            Ship *partOfShip() const { return _type == AreaType::Ship ? _partOfShip : nullptr; }
            StarSystem *partOfStarSystem() const { return _type == AreaType::StarSystem ? _partOfStarSystem : nullptr; }

            b2World *physicsWorld() const { return _physicsWorld.get(); }

            bool loopOver(LoopObjectCallback callback);

        private:
            // Fields
            AreaType _type;
            std::vector<SpaceObject *> _objects;
            std::map<SpaceObjectType, std::vector<SpaceObject *>> _groupedObjects;
            std::unique_ptr<b2World> _physicsWorld;
            SpaceObject *_partOfObject;
            sf::Transform _transform;

            AreaQuadtree _quadtree;
            std::vector<SpaceObject *> _notInQuadTree;

            std::vector<SpaceObject *> _lastDrawnObjects;

            union
            {
                Ship *_partOfShip;
                PlanetSurface *_partOfPlanetSurface;
                StarSystem *_partOfStarSystem;
            };

            // Methods
            static bool sortByPosition(SpaceObject *obj1, SpaceObject *obj2);
    };
} // space