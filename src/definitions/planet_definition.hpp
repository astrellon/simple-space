#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "celestial_body_definition.hpp"

namespace space
{
    class Engine;

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

            const sf::Texture *texture;

            // Methods
            static const std::string DefinitionType() { return "planet"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);

            // Constructor
            PlanetDefinition(DefinitionId id) : CelestialBodyDefinition(id), oscillateNoise(0.0f) { }
            virtual ~PlanetDefinition() { }
    };
} // space