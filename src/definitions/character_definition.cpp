#include "character_definition.hpp"

#include "../engine.hpp"
#include "animated_texture.hpp"
#include "../definitions/compendium_definition.hpp"

namespace space
{
    void CharacterDefinition::onPostLoad(Engine &engine)
    {
        engine.definitionManager().tryGet(animatedTextureId, &texture);

        if (compendiumId.size() > 0)
        {
            engine.definitionManager().tryGet(compendiumId, &compendiumDef);
        }
    }
} // namespace space
