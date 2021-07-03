#pragma once

#include <vector>

#include "../ui_element.hpp"

#include "../../game/items/placed_item.hpp"

namespace space
{
    class Teleporter;
    class UITextElement;
    class UIButton;

    class GameUITeleporter : public UIElement
    {
        public:
            // Fields

            // Constructor
            GameUITeleporter() : _uiManager(nullptr), _teleporter(nullptr, nullptr), _text(nullptr), _actionButton(nullptr) { }

            // Methods
            virtual void init(GameUIManager &uiManager);

            void teleporter(PlacedItemPair<Teleporter> teleporter);
            PlacedItemPair<Teleporter> teleporter() const { return _teleporter; }
            virtual const char*elementType() const { return "GameUITeleporter"; }

        private:
            // Fields
            GameUIManager *_uiManager;
            PlacedItemPair<Teleporter> _teleporter;
            UITextElement *_text;
            UIButton *_actionButton;

            // Methods
    };
} // space