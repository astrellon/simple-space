#pragma once

#include <SFML/Graphics.hpp>

#include "space_object.hpp"
#include "../definitions/celestial_body_definition.hpp"

namespace space
{
    class Engine;

    class CelestialBody : public SpaceObject
    {
        public:
            // Fields
            const CelestialBodyDefinition &celestialBodyDefinition;

            // Constructor
            CelestialBody(const ObjectId &id, const CelestialBodyDefinition &definition) : SpaceObject(id), celestialBodyDefinition(definition) { }

            // Methods
            //virtual void update(Engine &engine, sf::Time dt) = 0;
            //virtual void draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform);

        protected:
            // Methods
            void setTransformFromLocation();
    };
} // space