#pragma once

#include <memory>
#include <SFML/System.hpp>

#include "base_game_scene.hpp"
#include "star_background.hpp"

namespace space
{
    class Engine;
    class StarBackground;

    class MainMenuScene : public BaseGameScene
    {
        public:
            // Fields

            // Constructor
            MainMenuScene(Engine &engine);

            // Methods
            virtual void update(sf::Time dt);
            virtual void draw();

        private:
            // Fields
            Engine &_engine;
            std::unique_ptr<StarBackground> _background;

            // Methods
    };
} // space