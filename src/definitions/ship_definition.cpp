#include "ship_definition.hpp"

#include "../engine.hpp"

namespace space
{
    void ShipDefinition::onPostLoad(Engine &engine)
    {
        texture = engine.textureManager().get(texturePath);
    }
}