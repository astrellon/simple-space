#pragma once

#include <string>
//#include "../non_copyable.hpp"

namespace space
{
    typedef std::string DefinitionId;

    class BaseDefinition
    {
        public:
            // Fields
            DefinitionId id;

            virtual std::string type() const = 0;
    };
} // space