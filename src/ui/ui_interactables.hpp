#pragma once

namespace space
{
    class GameSession;
    class PlayerController;

    class UIInteractables
    {
        public:
            static void drawInteractables(GameSession &session);

        private:
            UIInteractables();
    };
} // space