#include "teleporter_list.hpp"

#include <algorithm>

namespace space
{
    TeleporterListPair::TeleporterListPair(PlacedItemPair<Teleporter> teleporter, int mark) : teleporter(teleporter), mark(mark)
    {

    }

    void TeleporterList::addTeleporter(PlacedItemPair<Teleporter> teleporter)
    {
        auto find = findTeleporter(teleporter);
        if (find == _teleporters.end())
        {
            _teleporters.emplace_back(teleporter, _marker);
            onAddTeleporter.emit(teleporter);
        }
    }

    void TeleporterList::removeTeleporter(PlacedItemPair<Teleporter> teleporter)
    {
        auto find = findTeleporter(teleporter);
        if (find != _teleporters.end())
        {
            _teleporters.erase(find);
            onRemoveTeleporter.emit(teleporter);
        }
    }

    void TeleporterList::clear()
    {
        for (auto teleporterPair : _teleporters)
        {
            onRemoveTeleporter.emit(teleporterPair.teleporter);
        }

        _teleporters.clear();
    }

    std::vector<TeleporterListPair>::iterator TeleporterList::findTeleporter(const PlacedItemPair<Teleporter> &teleporter)
    {
        return std::find_if(_teleporters.begin(), _teleporters.end(),
        [&teleporter](const TeleporterListPair &pair) -> bool {
            return pair.teleporter.placed == teleporter.placed;
        });
    }
} // space