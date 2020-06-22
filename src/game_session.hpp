#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "definitions/ship_definition.hpp"
#include "definitions/planet_definition.hpp"
#include "game/space_object.hpp"

#include "map-layer.hpp"

namespace space
{
    class Ship;
    class Planet;
    class Engine;
    class StarSystem;

    class GameSession
    {
        public:
            GameSession(Engine &engine);
            ~GameSession();

            Ship *createShip(const ObjectId &id, const ShipDefinition &definition);
            Planet *createPlanet(const ObjectId &id, const PlanetDefinition &definition);
            StarSystem *createStarSystem(const ObjectId &id, const PlanetDefinition &definition);

            bool tryGetSpaceObject(const ObjectId &id, SpaceObject **result);

            void update(sf::Time dt);
            void draw(sf::RenderTarget &target);

        private:
            Engine &_engine;

            std::vector<std::unique_ptr<SpaceObject>> _spaceObjects;
            std::unique_ptr<MapLayer> _mapLayer;

            StarSystem *_activeStarSystem;

    };
} // town