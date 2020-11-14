#include "planet_definition.hpp"

#include "../engine.hpp"
#include "../definition_manager.hpp"
#include "../definitions/compendium_definition.hpp"
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

        if (compendiumId.size() > 0)
        {
            if (!engine.definitionManager().tryGet(compendiumId, &compendiumDef))
            {
                std::cout << "Failed to find compendium id: " << compendiumId << " for planet: " << id << std::endl;
            }
        }
    }
}