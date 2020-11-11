#include "compendium.hpp"

#include "definitions/compendium_definition.hpp"
#include "game/live_photo.hpp"
#include "utils.hpp"

namespace space
{
    CompendiumEntry::CompendiumEntry(const CompendiumDefinition &definition) : definition(definition)
    {

    }

    void CompendiumEntry::addPhoto(LivePhoto *photo)
    {
        if (photo == nullptr || Utils::contains(_linkedPhotos, photo))
        {
            return;
        }

        _linkedPhotos.push_back(photo);
    }
    void CompendiumEntry::removePhoto(const ObjectId &id)
    {
        for (auto iter = _linkedPhotos.begin(); iter != _linkedPhotos.end(); ++iter)
        {
            if ((*iter)->id == id)
            {
                _linkedPhotos.erase(iter);
                return;
            }
        }
    }

    CompendiumEntry &Compendium::unlockEntry(const CompendiumDefinition &definition)
    {
        CompendiumEntry *currentEntry;
        if (tryGetEntry(definition.id, &currentEntry))
        {
            return *currentEntry;
        }

        auto result = _entries.emplace(std::make_pair(definition.id, CompendiumEntry(definition)));

        return result.first->second;
    }

    bool Compendium::tryGetEntry(const DefinitionId &compendiumDefId, CompendiumEntry **result)
    {
        auto find = _entries.find(compendiumDefId);
        if (find != _entries.end())
        {
            *result = &find->second;
            return true;
        }

        return false;
    }

    bool Compendium::tryGetEntry(const DefinitionId &compendiumDefId, const CompendiumEntry **result) const
    {
        auto find = _entries.find(compendiumDefId);
        if (find != _entries.end())
        {
            *result = &find->second;
            return true;
        }

        return false;
    }

    void Compendium::processNewPhoto(LivePhoto &photo)
    {

    }
} // space