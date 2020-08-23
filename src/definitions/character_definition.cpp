#include "character_definition.hpp"

#include "../engine.hpp"
#include "animated_texture.hpp"

namespace space
{
    void CharacterDefinition::onPostLoad(Engine &engine)
    {
        engine.definitionManager().tryGet(animatedTextureId, &texture);
    }
} // namespace space
