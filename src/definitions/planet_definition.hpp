#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "base_definition.hpp"

namespace space
{
    class Engine;

    class PlanetDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string texturePath;
            std::string name;

            float rotationRate;
            float size;

            const sf::Texture *texture;

            // Methods
            static const std::string DefinitionType() { return "planet"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);

            // Constructor
            PlanetDefinition(DefinitionId id) : BaseDefinition(id) { }
    };
} // space