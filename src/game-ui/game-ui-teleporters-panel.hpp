#pragma once

#include <observe/observer.h>

#include "./game-ui-window.hpp"
#include "../game/items/placed_item.hpp"

namespace space
{
    class Teleporter;
    class TeleporterList;
    class GameUITeleporter;
    class UINineSliceImageElement;

    class GameUITeleportersPanel : public UIElement
    {
        public:
            // Fields

            // Constructor
            GameUITeleportersPanel() : UIElement(), _teleporters(nullptr) { }

            // Methods
            virtual void init(GameUIManager &uiManager);
            virtual void draw(Engine &engine, RenderCamera &target);
            virtual void update(Engine &engine, sf::Time dt, sf::Vector2f parentOffset);

            void teleporters(const TeleporterList *teleporters);
            const TeleporterList *teleporters() const { return _teleporters; }

            virtual const char*elementType() const { return "GameUITeleporters"; }

        private:
            // Fields
            const TeleporterList *_teleporters;
            observe::Observer _removeOnAddTeleporter;
            observe::Observer _removeOnRemoveTeleporter;
            GameUIManager *_uiManager;

            UINineSliceImageElement *_backPanel;
            UIElement *_bodyContainer;

            std::vector<GameUITeleporter *> _teleporterUIs;

            // Methods
            void addTeleporter(PlacedItemPair<Teleporter> item);
            void removeTeleporter(PlacedItemPair<Teleporter> item);
    };
} // space