#pragma once

#include "engine.hpp"
#include "game_session.hpp"
#include "player_controller.hpp"
#include "character.hpp"
#include "ship.hpp"
#include "walkable_area.hpp"

namespace space
{
    class Helper
    {
        public:
            // Fields

            // Methods
            static inline bool isControllingCharacter(Engine &engine)
            {
                return engine.currentSession()->playerController().controlling() == ControlCharacter;
            }

            static inline Ship *getPlayerShip(Engine &engine)
            {
                return engine.currentSession()->playerController().controllingShip();
            }

            static inline Ship *getShipPlayerIsInsideOf(Engine &engine)
            {
                const auto character = engine.currentSession()->playerController().controllingCharacter();
                if (character == nullptr)
                {
                    return nullptr;
                }

                const auto area = character->insideArea();
                if (area == nullptr)
                {
                    return nullptr;
                }

                return area->partOfShip();
            }

        private:
            Helper();
    };
} // space