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

    class Planet : public CelestialBody
    {
        public:
            typedef std::vector<PlanetSurface *> PlanetSurfaceList;

            // Fields
            const PlanetDefinition &definition;

            // Constructor
            Planet(const ObjectId &id, const PlanetDefinition &definition);

            // Methods
            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, sf::RenderTarget &target);

            void addPlanetSurface(PlanetSurface *planetSurface);
            void removePlanetSurface(PlanetSurface *PlanetSurface);
            const PlanetSurfaceList &planetSurfaces() const { return _planetSurfaces; }

        private:
            // Fields
            std::unique_ptr<sf::RenderTexture> _renderTexture;
            sf::Shader *_shader;
            PlanetSurfaceList _planetSurfaces;

            // Methods
    };
} // space