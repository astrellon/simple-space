#include "teleporter_list.hpp"

#include <algorithm>
#include <limits>

namespace space
{
    TeleporterListPair::TeleporterListPair(PlacedItemPair<Teleporter> teleporter, uint mark) : teleporter(teleporter), mark(mark)
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
        else
        {
            find->mark = _marker;
        }
    }

    void TeleporterList::removeTeleporter(PlacedItemPair<Teleporter> teleporter)
    {
        auto find = findTeleporter(teleporter);
        if (find != _teleporters.end())
        {
            std::cout << "Remove teleporter: " << teleporter.item->name() << std::endl;
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

    void TeleporterList::updateMark()
    {
        _marker++;
        if (_marker == UINT_MAX)
        {
            _marker = 0u;
        }
    }

    void TeleporterList::removeOldMarked()
    {
        for (int i = (int)_teleporters.size() - 1; i >= 0; i--)
        {
            auto iter = _teleporters.begin() + i;
            if (iter->mark < _marker)
            {
                auto teleporter = iter->teleporter;
                std::cout << "Remove teleporter: " << teleporter.item->name() << std::endl;
                _teleporters.erase(iter);
                onRemoveTeleporter.emit(teleporter);
            }
        }
    }

    std::vector<TeleporterListPair>::iterator TeleporterList::findTeleporter(const PlacedItemPair<Teleporter> &teleporter)
    {
        for (auto iter = _teleporters.begin(); iter != _teleporters.end(); ++iter)
        {
            if (iter->teleporter.item == teleporter.item)
            {
                return iter;
            }
        }
        return _teleporters.end();
    }
} // space