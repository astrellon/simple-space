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
            // Types
            typedef std::vector<LivePhoto *> PhotoList;

            // Fields
            const CompendiumDefinition &definition;

            // Constructor
            CompendiumEntry(const CompendiumDefinition &definition);

            // Methods
            void addPhoto(LivePhoto *photo);
            void removePhoto(const ObjectId &id);

            PhotoList &photos() { return _linkedPhotos; }
            const PhotoList &photos() const { return _linkedPhotos; }

        private:
            // Fields
            PhotoList _linkedPhotos;

            // Methods
    };

    class Compendium : private NonCopyable
    {
        public:
            // Types
            typedef std::map<DefinitionId, CompendiumEntry> EntryMap;
            // Fields

            // Constructor

            // Methods
            CompendiumEntry &unlockEntry(const CompendiumDefinition &definition);

            bool tryGetEntry(const DefinitionId &compendiumDefId, CompendiumEntry **result);
            bool tryGetEntry(const DefinitionId &compendiumDefId, const CompendiumEntry **result) const;

            void processNewPhoto(LivePhoto *photo);

            EntryMap &entries() { return _entries; }
            const EntryMap &entries() const { return _entries; }

        private:
            // Fields
            EntryMap _entries;

            // Methods
    };
} // space