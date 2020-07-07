#pragma once

#include <vector>

namespace space
{
    class Ship;
    class GameSession;

    class UITeleporter
    {
        public:
            static void draw(GameSession &session, const std::vector<Ship *> &ships);

        private:
            UITeleporter();
    };
} // space