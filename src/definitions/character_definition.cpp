#include "character_definition.hpp"

#include "../engine.hpp"

namespace space
{
    void CharacterDefinition::onPostLoad(Engine &engine)
    {
        engine.resourceManager().texture(texturePath, &texture);
        tiles.init(texture, spriteSize);
    }
} // namespace space
