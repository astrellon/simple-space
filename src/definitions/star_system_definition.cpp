#include "star_system_definition.hpp"

#include "../engine.hpp"

namespace space
{
    void StarSystemDefinition::onPostLoad(Engine &engine)
    {
        rootBody->onPostLoad(engine);
    }
} // namespace space
