#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "space_object.hpp"
#include "celestial_body.hpp"
#include "../definitions/planet_definition.hpp"

namespace space
{
    class GameSession;
    class PlanetSurface;
    class LoadingContext;
    class RenderCamera;

    class Planet : public CelestialBody
    {
        public:
            static const SpaceObjectType2 TypeValue;

            typedef std::vector<PlanetSurface *> PlanetSurfaceList;

            // Fields
            const PlanetDefinition &definition;

            // Constructor
            Planet(const ObjectId &id, const PlanetDefinition &definition);

            // Methods
            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) { return clonePlanet(newId, context); }
            Planet *clonePlanet(const ObjectId &newId, const CloneContext &context);

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual bool doesMouseHover(GameSession &session, sf::Vector2f mousePosition) const;
            virtual bool isGenerated() const { return true; }
            virtual DrawLayers::Type drawLayer() const { return DrawLayers::Background; }

            void addPlanetSurface(PlanetSurface *planetSurface);
            void removePlanetSurface(PlanetSurface *PlanetSurface);
            const PlanetSurfaceList &planetSurfaces() const { return _planetSurfaces; }

            virtual const CompendiumDefinition *compendiumDefinition() const { return definition.compendiumDef; }

        private:
            // Fields
            std::unique_ptr<sf::RenderTexture> _renderTexture;
            sf::Shader *_shader;
            PlanetSurfaceList _planetSurfaces;
            bool _initedSurfaces;

            // Methods
    };
} // space