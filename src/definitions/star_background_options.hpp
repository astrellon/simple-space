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
            std::string starColoursPath;
            sf::Color backgroundColour;
            int numParticles;
            float area;

            sf::Shader *shader;
            const sf::Image *starColours;

            // Constructor
            StarBackgroundOptions();

            // Methods
            void onPostLoad(Engine &engine);
    };
} // space