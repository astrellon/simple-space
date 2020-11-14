#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

#include "celestial_body_definition.hpp"

namespace space
{
    class Engine;
    class PlanetSurfaceDefinition;
    class CompendiumDefinition;

    class PlanetDefinition : public CelestialBodyDefinition
    {
        public:
            // Fields
            std::string texturePath;
            std::string name;
            sf::Color glowColour;

            float rotationRate;
            float size;
            float scale;
            float oscillateNoise;

            DefinitionIds planetSurfaceIds;
            DefinitionId compendiumId;

            // Post Load Fields
            const sf::Texture *texture;
            const CompendiumDefinition *compendiumDef;

            // Constructor
            PlanetDefinition(DefinitionId id) : CelestialBodyDefinition(id), oscillateNoise(0.0f), compendiumDef(nullptr) { }
            virtual ~PlanetDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "planet"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);
    };
} // space