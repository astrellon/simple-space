#pragma once

namespace space
{
    class GameSession;

    class UIInteractables
    {
        public:
            static void draw(GameSession &session);

        private:
            UIInteractables();
    };
} // space