#include "celestial_body_definition.hpp"

namespace space
{
    void CelestialBodyDefinition::onPostLoad(Engine &engine)
    {
        for (auto &child : children)
        {
            child->onPostLoad(engine);
        }
    }
} // namespace space
