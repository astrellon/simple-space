#pragma once

#include "interaction.hpp"

namespace space
{
    class GameSession;
    class PlacedItem;

    class UseItemAction : public Interaction
    {
        public:
            // Fields

            // Constructor
            UseItemAction(PlacedItem *item);

            // Methods
            virtual std::string label() const { return "Use"; }
            virtual void execute(GameSession &session);

        private:
            PlacedItem *_placedItem;
    };
} // space