#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "../resource_manager.hpp"

namespace space
{
    class TeleportScreenEffect
    {
        public:
            // Constructor
            TeleportScreenEffect();

            // Methods
            void init(ResourceManager &resourceManager);
            void draw(const sf::Texture *texture, sf::RenderTarget &target, float t);

        private:
            sf::Shader *_dissolve;
            sf::VertexArray _vertices;

    };

} // namespace space