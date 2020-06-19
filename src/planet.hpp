#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "game/space_object.hpp"

namespace space
{
    class Engine;

    class Planet : public SpaceObject
    {
        public:
            // Fields

            // Constructor
            Planet(Engine &engine, sf::Texture *texture);

            // Methods
            virtual void update(sf::Time dt) {}
            virtual void draw(sf::RenderTarget &target, const sf::Transform &parentTransform);

        private:
            // Fields
            Engine &_engine;
            std::unique_ptr<sf::RenderTexture> _renderTexture;
            sf::Texture *_texture;
            sf::Shader *_shader;

            // Methods
    };
} // space