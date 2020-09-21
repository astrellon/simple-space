#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "base_definition.hpp"

namespace space
{
    class ShaderDefinition;

    class GrassEffectDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string texturePath;
            std::string shaderId;
            sf::Color tipColour;
            sf::Color sideColour;
            sf::Color windColour;

            // PostLoad
            const sf::Texture *texture;
            ShaderDefinition *shader;

            // Constructor
            GrassEffectDefinition(DefinitionId id) : BaseDefinition(id), texture(nullptr) { }
            virtual ~GrassEffectDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "grass-effect"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);
    };
} // space