#pragma once

#include <map>
#include <memory>

#include "definitions/base_definition.hpp"
#include "non_copyable.hpp"

namespace space
{
    class CompendiumEntry
    {
        public:
            // Fields
            bool discovered;

            // Where and when discovered
            // Texture to photo?

            // Constructor

            // Methods
    };

    class Compendium : private NonCopyable
    {
        public:
            // Fields

            // Constructor

            // Methods

        private:
            // Fields
            std::map<DefinitionId, CompendiumEntry> _entries;

            // Methods
    };
} // space