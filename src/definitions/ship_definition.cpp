#include "ship_definition.hpp"

#include "../engine.hpp"

namespace space
{
    void ShipDefinition::on_post_load(Engine &engine)
    {
        texture = engine.textureManager().get(texturePath);
    }
}