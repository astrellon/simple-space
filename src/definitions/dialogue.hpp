#pragma once

#include <string>
#include <vector>

#include "base_definition.hpp"

namespace space
{
    class Dialogue : public BaseDefinition
    {
        public:
            // Fields
            const std::vector<std::string> text;

            // Constructor
            Dialogue(DefinitionId id, const std::vector<std::string> &text) : BaseDefinition(id), text(text) { }
            virtual ~Dialogue() { }

            // Methods
            static const std::string DefinitionType() { return "dialogue"; }
            std::string type() const { return DefinitionType(); }
    };
} // space