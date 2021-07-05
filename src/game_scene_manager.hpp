#pragma once

#include <memory>

namespace space
{
    class Engine;
    class MainMenuScene;
    class GameSession;

    class GameSceneManager
    {
        public:
            // Fields

            // Constructor

            // Methods
            static void switchToMainMenu(Engine &engine);
            static void startNewGame(Engine &engine);

        private:
            GameSceneManager();
    };
} // space