#pragma once

#include <SFML/Graphics.hpp>

namespace space
{
    class Engine;

    class Overlay
    {
        public:
            // Fields

            // Constructor
            Overlay(Engine &engine);

            // Methods
            void draw(sf::RenderTarget &output, float alpha);

        private:
            // Fields
            sf::VertexArray _vertices;
            sf::Shader *_shader;

            // Methods
    };
} // space