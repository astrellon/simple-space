#pragma once

#include <SFML/Graphics.hpp>

namespace space
{
    class DefinitionManager;

    class Overlay
    {
        public:
            // Fields

            // Constructor
            Overlay();

            // Methods
            bool init(DefinitionManager &definitionManager);
            void draw(sf::RenderTarget &output, float alpha);

        private:
            // Fields
            sf::VertexArray _vertices;
            sf::Shader *_shader;

            // Methods
    };
} // space