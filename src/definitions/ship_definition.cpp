#include "ship_definition.hpp"

#include "../engine.hpp"

namespace space
{
    void ShipDefinition::onPostLoad(Engine &engine)
    {
        engine.resourceManager().texture(texturePath, &texture);
    }
}