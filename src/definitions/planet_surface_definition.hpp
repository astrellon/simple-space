#pragma once

#include <string>
#include <memory>

#include "base_definition.hpp"

namespace tmx
{
    class Map;
}

namespace space
{
    class Engine;

    class PlanetSurfaceDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string name;
            std::string tmxMapPath;

            tmx::Map *tmxMap;
            // Constructor
            PlanetSurfaceDefinition(DefinitionId id) : BaseDefinition(id) { }
            virtual ~PlanetSurfaceDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "planet-surface"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);
    };
} // space