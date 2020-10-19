#include "compendium_definition.hpp"

#include "../engine.hpp"

namespace space
{
    void CompendiumDefinition::onPostLoad(Engine &engine)
    {
        if (picturePath.size() > 0)
        {
            engine.definitionManager().tryGet(picturePath, &picture);
        }
    }
} // space