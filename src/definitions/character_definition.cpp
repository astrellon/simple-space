#include "character_definition.hpp"

#include "../engine.hpp"
#include "animated_texture.hpp"
#include "../definitions/compendium_definition.hpp"
#include "../definitions/dialogue_audio.hpp"

namespace space
{
    void CharacterDefinition::onPostLoad(Engine &engine)
    {
        if (!engine.definitionManager().tryGet(animatedTextureId, &texture))
        {
            std::cout << "Failed to find animated texture id: " << animatedTextureId << " for char: " << id << std::endl;
        }

        if (dialogueAudioId.size() > 0)
        {
            if (!engine.definitionManager().tryGet(dialogueAudioId, &dialogueAudio))
            {
                std::cout << "Failed to find dialogue audio id: " << dialogueAudioId << " for char: " << id << std::endl;
            }
        }

        if (compendiumId.size() > 0)
        {
            if (!engine.definitionManager().tryGet(compendiumId, &compendiumDef))
            {
                std::cout << "Failed to find compendium id: " << compendiumId << " for char: " << id << std::endl;
            }
        }
    }
} // namespace space
