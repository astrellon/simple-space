#pragma once

#include <string>
#include "base_definition.hpp"

namespace space
{
    class ShipDefinition : public BaseDefinition
    {
        public:
            //const static std::string DefinitionType;

            // Fields
            std::string texturePath;
            std::string name;
            float maxRotation;
            float maxSpeed;

            // Methods
            static const std::string DefinitionType() { return "ship"; }
            std::string type() const { return DefinitionType(); }
    };

    //const std::string ShipDefinition::DefinitionType = "ship";
} // space