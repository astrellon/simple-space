#pragma once

#include <string>

#include "base_definition.hpp"

namespace space
{
    class CompendiumDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string name;
            std::string species;
            std::string description;

            // Constructor
            CompendiumDefinition(DefinitionId id) : BaseDefinition(id) { }

            // Methods
    };
} // space