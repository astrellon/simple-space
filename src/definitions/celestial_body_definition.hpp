#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base_definition.hpp"

namespace space
{
    class Engine;

    class CelestialBodyLocation
    {
        public:
            // Fields
            float distance;
            float influenceRadius;
            float angle;
    };

    class CelestialBodyDefinition : public BaseDefinition
    {
        public:
            // Fields
            CelestialBodyLocation location;
            std::vector<std::unique_ptr<CelestialBodyDefinition>> children;

            // Constructor
            CelestialBodyDefinition(DefinitionId id) : BaseDefinition(id) { }
            virtual ~CelestialBodyDefinition() { };

            // Methods
            virtual std::string type() const = 0;

            virtual void onPostLoad(Engine &engine);
    };

    class OrbitPointCelestialDefinition : public CelestialBodyDefinition
    {
        public:
            // Fields

            // Constructor
            OrbitPointCelestialDefinition(DefinitionId id) : CelestialBodyDefinition(id) { }
            virtual ~OrbitPointCelestialDefinition() { }

            // Methods
            static const std::string DefinitionType() { return "orbit-point"; }
            virtual std::string type() const { return DefinitionType(); }
    };
} // space