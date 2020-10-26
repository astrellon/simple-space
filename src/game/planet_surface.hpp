#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../non_copyable.hpp"
#include "../definitions/planet_surface_definition.hpp"
#include "../map_layer.hpp"
#include "area.hpp"
#include "ihas_area.hpp"

namespace space
{
    class Character;
    class GameSession;
    class Planet;
    class RenderCamera;
    class LoadingContext;

    class PlanetSurface : public SpaceObject, public IHasArea
    {
        public:
            // Fields
            const PlanetSurfaceDefinition &definition;

            // Constructor
            PlanetSurface(const ObjectId &id, const PlanetSurfaceDefinition &definition);
            virtual ~PlanetSurface() { }

            // Methods
            virtual SpaceObject *clone(const ObjectId &newId, GameSession &session) { return clonePlanetSurface(newId, session); }
            virtual SpaceObject *deepClone(const ObjectId &newIdPrefix, GameSession &session);
            PlanetSurface *clonePlanetSurface(const ObjectId &newId, GameSession &session);

            static const std::string SpaceObjectType() { return PlanetSurfaceDefinition::DefinitionType(); }
            virtual std::string type() const { return SpaceObjectType(); }

            virtual Area &area() { return _area; }
            virtual const Area &area () const { return _area; }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransforms);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual void onPostLoad(GameSession &session, LoadingContext &context);
            virtual bool checkForMouse(GameSession &session, sf::Vector2f mousePosition);
            virtual bool doUpdateEveryFrame() const { return true; }

            void partOfPlanet(Planet *planet) { _partOfPlanet = planet; }
            Planet *partOfPlanet() const { return _partOfPlanet; }

        private:

            // Fields
            Area _area;
            Planet *_partOfPlanet;
            std::vector<std::unique_ptr<MapLayer>> _mapLayers;

            // Methods
            void createMapLayersFromDefinition(GameSession &session);
    };
} // space