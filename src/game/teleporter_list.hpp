#pragma once

#include <vector>
#include <observe/event.h>

#include "./items/placed_item.hpp"
#include "./items/teleporter.hpp"

namespace space
{
    struct TeleporterListPair
    {
        // Fields
        PlacedItemPair<Teleporter> teleporter;
        int mark;

        // Constructor
        TeleporterListPair(PlacedItemPair<Teleporter> teleporter, int mark);
    };

    class TeleporterList
    {
        public:
            typedef observe::Event<PlacedItemPair<Teleporter>> TeleporterHandlerHandler;

            // Fields
            TeleporterHandlerHandler onAddTeleporter;
            TeleporterHandlerHandler onRemoveTeleporter;

            // Constructor

            // Methods
            void addTeleporter(PlacedItemPair<Teleporter> teleporter);
            void removeTeleporter(PlacedItemPair<Teleporter> teleporter);

            void clear();
            void updateMark();
            void removeOldMarked();

            const std::vector<TeleporterListPair> &teleporters() const { return _teleporters; }

        private:
            // Fields
            std::vector<TeleporterListPair> _teleporters;
            int _marker;

            // Methods
            std::vector<TeleporterListPair>::iterator findTeleporter(const PlacedItemPair<Teleporter> &teleporter);
    };
} // space