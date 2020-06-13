#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "base_definition.hpp"

namespace space
{
    class Engine;

    class ShipDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string texturePath;
            std::string name;

            float maxRotation;
            float turnRate;
            float maxSpeed;
            float acceleration;

            const sf::Texture *texture;

            // Methods
            static const std::string DefinitionType() { return "ship"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);

            // Constructor
            ShipDefinition(DefinitionId id) :
            BaseDefinition(id) { }
    };
} // space