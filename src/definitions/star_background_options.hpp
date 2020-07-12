#pragma once

#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

namespace space
{
    class Engine;

    class StarBackgroundOptions
    {
        public:
            // Fields
            std::string shaderName;
            sf::Color backgroundColour;
            int numLayers;
            int numParticles;
            float area;

            sf::Shader *shader;

            // Constructor
            StarBackgroundOptions();

            // Methods
            void onPostLoad(Engine &engine);
    };
} // space