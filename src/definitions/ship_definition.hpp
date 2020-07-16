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
            std::string interiorTexturePath;
            std::string engineGlowTexturePath;
            sf::Vector2i interiorTextureOffset;
            std::string name;

            float maxRotation;
            float turnRate;
            float maxSpeed;
            float acceleration;

            const sf::Texture *texture;
            const sf::Texture *interiorTexture;
            const sf::Texture *engineGlowTexture;

            // Constructor
            ShipDefinition(const DefinitionId &id) : BaseDefinition(id), texture(nullptr), interiorTexture(nullptr), engineGlowTexture(nullptr) { }
            virtual ~ShipDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "ship"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);
    };
} // space