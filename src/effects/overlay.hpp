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
            Overlay(sf::Color colour = sf::Color(0.1 * 255, 0.15 * 255, 0.2 * 255, 255));

            // Methods
            bool init(DefinitionManager &definitionManager);
            void draw(sf::RenderTarget &output, float alpha);

        private:
            // Fields
            sf::VertexArray _vertices;
            sf::Shader *_shader;
            sf::Color _colour;

            // Methods
    };
} // space