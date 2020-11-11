#pragma once

#include <map>
#include <memory>
#include <vector>

#include "definitions/base_definition.hpp"
#include "non_copyable.hpp"
#include "types.hpp"

namespace space
{
    class LivePhoto;
    class CompendiumDefinition;

    class CompendiumEntry
    {
        public:
            // Fields
            const CompendiumDefinition &definition;

            // Constructor
            CompendiumEntry(const CompendiumDefinition &definition);

            // Methods
            void addPhoto(LivePhoto *photo);
            void removePhoto(const ObjectId &id);

        private:
            // Fields
            std::vector<LivePhoto *> _linkedPhotos;

            // Methods
    };

    class Compendium : private NonCopyable
    {
        public:
            // Fields

            // Constructor

            // Methods
            CompendiumEntry &unlockEntry(const CompendiumDefinition &definition);

            bool tryGetEntry(const DefinitionId &compendiumDefId, CompendiumEntry **result);
            bool tryGetEntry(const DefinitionId &compendiumDefId, const CompendiumEntry **result) const;

            void processNewPhoto(LivePhoto &photo);

        private:
            // Fields
            std::map<DefinitionId, CompendiumEntry> _entries;

            // Methods
    };
} // space