#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "base_definition.hpp"
#include "../tiles.hpp"

namespace space
{
    class Engine;

    class CharacterDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string texturePath;
            uint spriteSize;

            const sf::Texture *texture;
            Tiles tiles;

            // Constructor
            CharacterDefinition(const DefinitionId &id) : BaseDefinition(id) { }
            virtual ~CharacterDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "character"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);
    };
} // space