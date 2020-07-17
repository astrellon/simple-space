#pragma once

#include <SFML/Graphics.hpp>

namespace space
{
    class GameSession;
    class Ship;

    class EngineFlameEffect
    {
        public:
            // Fields

            // Constructor
            EngineFlameEffect(Ship &parentShip, const sf::Texture &texture, sf::Vector2f position);

            // Methods
            void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            void draw(sf::RenderTarget &target);

        private:
            // Fields
            Ship &_parentShip;
            sf::Sprite _glow;
            sf::Vector2f _position;
            float _engineBrightness;

            // Methods
    };
} // space