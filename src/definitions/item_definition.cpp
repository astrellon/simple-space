#include "item_definition.hpp"

#include "../engine.hpp"

namespace space
{
    void ItemDefinition::onPostLoad(Engine &engine)
    {
        engine.resourceManager().texture(texturePath, &texture);
    }
} // namespace space
