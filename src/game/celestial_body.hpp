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
            CelestialBody(const ObjectId &id, const CelestialBodyDefinition &definition, SpaceObjectType t) : SpaceObject(id, t), celestialBodyDefinition(definition) { }

            // Methods

        protected:
            // Methods
            void setTransformFromLocation();
    };
} // space