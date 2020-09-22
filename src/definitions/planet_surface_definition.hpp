#pragma once

#include <string>
#include <memory>
#include <vector>

#include "base_definition.hpp"

#include "../game/walkable_area_instances.hpp"

namespace tmx
{
    class Map;
}

namespace space
{
    class Engine;
    class GrassEffectDefinition;

    class PlanetSurfaceDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string name;
            std::string tmxMapPath;

            tmx::Map *tmxMap;
            WalkableAreaInstances walkableAreaInstances;

            // Constructor
            PlanetSurfaceDefinition(DefinitionId id) : BaseDefinition(id) { }
            virtual ~PlanetSurfaceDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "planet-surface"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);
    };
} // space