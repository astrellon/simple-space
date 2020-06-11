#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

namespace space
{
    class Ship;
    class Engine;

    class GameSession
    {
        public:
            GameSession(Engine *engine);
            ~GameSession();

            Ship *createShip(const sf::Texture *texture, float maxRotation, float maxSpeed);

            void onResize(sf::Vector2f area);

            void update(sf::Time dt);
            void draw(sf::RenderTarget &target);

        private:
            Engine *_engine;
            sf::View _camera;

            std::vector<std::unique_ptr<Ship>> _ships;

    };
} // town