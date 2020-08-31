#include "space_portal_definition.hpp"

#include "../engine.hpp"
#include "../definitions/animated_texture.hpp"

namespace space
{
    void SpacePortalDefinition::onPostLoad(Engine &engine)
    {
        engine.definitionManager().tryGet(animatedTextureId, &texture);
    }
} // namespace space