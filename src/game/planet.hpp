#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "space_object.hpp"
#include "../definitions/planet_definition.hpp"

namespace space
{
    class Engine;

    class Planet : public SpaceObject
    {
        public:
            // Fields
            const PlanetDefinition &definition;

            // Constructor
            Planet(const ObjectId &id, const PlanetDefinition &definition);

            // Methods
            virtual void update(Engine &engine, sf::Time dt) {}
            virtual void draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform);

        private:
            // Fields
            std::unique_ptr<sf::RenderTexture> _renderTexture;
            sf::Shader *_shader;

            // Methods
    };
} // space