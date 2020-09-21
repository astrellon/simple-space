#include "grass_effect_definition.hpp"

#include "../engine.hpp"
#include "shader_definition.hpp"

namespace space
{
    void GrassEffectDefinition::onPostLoad(Engine &engine)
    {
        engine.definitionManager().tryGet(shaderId, &shader);
        engine.resourceManager().texture(texturePath, &texture);
    }
} // space