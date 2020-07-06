#pragma once

namespace space
{
    class PlayerController;

    class UIInventory
    {
        public:
            static void draw(PlayerController &player);

        private:
            UIInventory();
    };
} // space