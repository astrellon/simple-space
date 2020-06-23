#pragma once

#include <SFML/Graphics.hpp>

#include "space_object.hpp"
#include "../definitions/character_definition.hpp"
#include "../tiles.hpp"

namespace space
{
    class Engine;

    class Character : public SpaceObject
    {
        public:
            // Fields
            const CharacterDefinition &definition;

            // Constructor
            Character(const ObjectId &id, const CharacterDefinition &definition);

            // Methods
            virtual void update(Engine &engine, sf::Time dt);
            virtual void draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform);

        private:
            // Fields
            sf::Clock _timeSinceStart;

            // Methods
    };
} // space