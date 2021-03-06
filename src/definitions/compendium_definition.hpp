#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "base_definition.hpp"

namespace space
{
    class CompendiumDefinition : public BaseDefinition
    {
        public:
            // Fields
            std::string name;
            std::string description;

            // Constructor
            CompendiumDefinition(DefinitionId id) : BaseDefinition(id) { }

            // Methods
            static const std::string DefinitionType() { return "compendium"; }
            std::string type() const { return DefinitionType(); }

            virtual void onPostLoad(Engine &engine);
    };
} // space