#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

namespace space
{
    class GameSession;
    class DefinitionManager;

    class TeleportScreenEffect
    {
        public:
            // Constructor
            TeleportScreenEffect();

            // Methods
            bool init(DefinitionManager &definitionManager);
            void draw(GameSession &session, const sf::Texture *texture, sf::RenderTarget &target, float t);
            void offset(float value) { _offset = value; }

        private:
            sf::Shader *_dissolve;
            sf::VertexArray _vertices;
            float _offset;

    };

} // namespace space