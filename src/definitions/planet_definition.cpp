#include "planet_definition.hpp"

#include "../engine.hpp"
#include "../definition_manager.hpp"
#include "planet_surface_definition.hpp"

namespace space
{
    void PlanetDefinition::onPostLoad(Engine &engine)
    {
        CelestialBodyDefinition::onPostLoad(engine);

        sf::Texture *text;
        engine.resourceManager().texture(texturePath, &text);
        texture = text;

        text->setRepeated(true);
        text->setSmooth(false);
    }
}