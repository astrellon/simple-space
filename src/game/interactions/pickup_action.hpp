#pragma once

#include "interaction.hpp"

namespace space
{
    class GameSession;
    class PlacedItem;

    class PickupAction : public Interaction
    {
        public:
            // Fields

            // Constructor
            PickupAction(PlacedItem *item);

            // Methods
            virtual std::string label() const { return "Pickup"; }
            virtual void execute(GameSession &session);

        private:
            PlacedItem *_placedItem;
    };
} // space