#pragma once

#include <string>
#include "base_definition.hpp"

namespace space
{
    class ShipDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string texturePath;
            std::string name;
            float maxRotation;
            float maxSpeed;

            // Methods
            static const std::string DefinitionType() { return "ship"; }
            std::string type() const { return DefinitionType(); }

            // Constructor
            ShipDefinition(DefinitionId id) : BaseDefinition(id) { }
    };
} // space