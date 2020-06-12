#pragma once

#include <iostream>
#include <string>

#include "../non_copyable.hpp"

namespace space
{
    typedef std::string DefinitionId;
    class Engine;

    class BaseDefinition : private NonCopyable
    {
        public:
            // Fields
            const DefinitionId id;

            virtual std::string type() const = 0;

            // Constructor
            BaseDefinition(const DefinitionId &id);

            virtual void onPostLoad(Engine &engine);
    };
} // space