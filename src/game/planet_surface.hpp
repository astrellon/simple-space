#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../non_copyable.hpp"
#include "../definitions/planet_surface_definition.hpp"
#include "../map_layer.hpp"
#include "area.hpp"

namespace space
{
    class Character;
    class GameSession;
    class Planet;
    class RenderCamera;
    class LoadingContext;

    class PlanetSurface : public SpaceObject
    {
        public:
            // Fields
            const PlanetSurfaceDefinition &definition;

            // Constructor
            PlanetSurface(const PlanetSurfaceDefinition &definition);
            virtual ~PlanetSurface() { }

            // Methods
            static const std::string SpaceObjectType() { return PlanetSurfaceDefinition::DefinitionType(); }
            virtual std::string type() const { return SpaceObjectType(); }

            Area &area() { return _area; }
            const Area &area () const { return _area; }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransforms);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual void onPostLoad(GameSession &session, LoadingContext &context);
            virtual bool checkForMouse(GameSession &session, sf::Vector2f mousePosition);

            void partOfPlanet(Planet *planet) { _partOfPlanet = planet; }
            Planet *partOfPlanet() const { return _partOfPlanet; }

        private:

            // Fields
            Area _area;
            Planet *_partOfPlanet;
            std::vector<std::unique_ptr<MapLayer>> _mapLayers;

            // Methods
    };
} // space