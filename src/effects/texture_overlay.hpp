#pragma once

#include <SFML/Graphics.hpp>

namespace space
{
    class DefinitionManager;

    class TextureOverlay
    {
        public:
            // Fields

            // Constructor
            TextureOverlay();

            // Methods
            bool init(DefinitionManager &definitionManager);
            void draw(sf::RenderTarget &output);

            void texture(const sf::Texture *texture) { _texture = texture; }

        private:
            // Fields
            sf::VertexArray _vertices;
            const sf::Texture *_texture;
            sf::Shader *_shader;

            // Methods
    };
} // space