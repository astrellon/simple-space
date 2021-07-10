#pragma once

#include <memory>

namespace space
{
    class Engine;

    class GameSceneManager
    {
        public:
            // Fields

            // Constructor

            // Methods
            static void switchToMainMenu(Engine &engine);
            static void startNewGame(Engine &engine);
            static void startEditor(Engine &engine);

        private:
            GameSceneManager();
    };
} // space