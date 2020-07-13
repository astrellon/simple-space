#pragma once

#include <vector>

namespace space
{
    class Ship;
    class Planet;
    class GameSession;

    class UITeleporter
    {
        public:
            static void draw(GameSession &session, const std::vector<Ship *> &ships, const std::vector<Planet *> &planets);

        private:
            UITeleporter();
    };
} // space