#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "base_definition.hpp"
#include "../tiles.hpp"

namespace space
{
    class Engine;
    class AnimatedTexture;
    class CompendiumDefinition;
    class DialogueAudio;

    class CharacterDefinition : public BaseDefinition
    {
        public:
            // Fields
            DefinitionId animatedTextureId;
            DefinitionId dialogueAudioId;
            DefinitionId compendiumId;

            std::string name;
            uint spriteSize;
            float speed;

            const AnimatedTexture *texture;
            const CompendiumDefinition *compendiumDef;
            DialogueAudio *dialogueAudio;

            // Constructor
            CharacterDefinition(const DefinitionId &id) : BaseDefinition(id), texture(nullptr), compendiumDef(nullptr), dialogueAudio(nullptr) { }
            virtual ~CharacterDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "character"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);
    };
} // space