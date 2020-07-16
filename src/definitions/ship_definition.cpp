#include "ship_definition.hpp"

#include "../engine.hpp"

namespace space
{
    void ShipDefinition::onPostLoad(Engine &engine)
    {
        engine.resourceManager().texture(texturePath, &texture);
        engine.resourceManager().texture(interiorTexturePath, &interiorTexture);

        if (engineGlowTexturePath.size() > 0)
        {
            engine.resourceManager().texture(engineGlowTexturePath, &engineGlowTexture);
        }
    }
}