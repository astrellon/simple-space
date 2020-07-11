#include "planet_surface_definition.hpp"

#include "../engine.hpp"

namespace space
{
    void PlanetSurfaceDefinition::onPostLoad(Engine &engine)
    {
        engine.resourceManager().map(tmxMapPath, &tmxMap);
    }
}