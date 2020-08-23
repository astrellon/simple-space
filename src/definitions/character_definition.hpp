#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "base_definition.hpp"
#include "../tiles.hpp"

namespace space
{
    class Engine;
    class AnimatedTexture;

    class CharacterDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string animatedTextureId;
            std::string name;
            uint spriteSize;
            float speed;

            const AnimatedTexture *texture;
            //Tiles tiles;

            // Constructor
            CharacterDefinition(const DefinitionId &id) : BaseDefinition(id) { }
            virtual ~CharacterDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "character"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);
    };
} // space