#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../non_copyable.hpp"
#include "../definitions/planet_surface_definition.hpp"
#include "walkable_area.hpp"
#include "../map_layer.hpp"

namespace space
{
    class Character;
    class GameSession;
    class Planet;
    class RenderCamera;

    class PlanetSurface : private NonCopyable
    {
        public:
            // Fields
            const PlanetSurfaceDefinition &definition;

            // Constructor
            PlanetSurface(GameSession &session, const PlanetSurfaceDefinition &definition);

            // Methods
            WalkableArea &walkableArea() { return _walkableArea; }
            const WalkableArea &walkableArea() const { return _walkableArea; }

            void update(sf::Time dt);
            void draw(RenderCamera &target);

            void partOfPlanet(Planet *planet) { _partOfPlanet = planet; }
            Planet *partOfPlanet() const { return _partOfPlanet; }

        private:

            // Fields
            GameSession &_session;
            WalkableArea _walkableArea;
            Planet *_partOfPlanet;
            std::vector<std::unique_ptr<MapLayer>> _mapLayers;

            // Methods
    };
} // space