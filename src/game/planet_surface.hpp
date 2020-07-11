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
            void draw(sf::RenderTarget &target);

        private:
            // Fields
            GameSession &_session;
            WalkableArea _walkableArea;
            std::unique_ptr<MapLayer> _mapLayer;

            // Methods
    };
} // space