#pragma once

#include <memory>

#include <SFML/System.hpp>

#include "game/space_object.hpp"

namespace space
{
    class StarSystem;
    class PlanetSurface;

    class TransitionData
    {
        public:
            // Fields
            StarSystem *starSystem;
            PlanetSurface *planetSurface;
            ObjectId followId;
            ObjectId followRotationId;
            float cameraScale;
            sf::Vector2f position;

            // Constructor
            TransitionData() : starSystem(nullptr), planetSurface(nullptr), cameraScale(1.0f) { }

            // Methods
    };

    class Transition
    {
        public:
            // Fields
            TransitionData fromData;
            TransitionData toData;

            sf::Time startTime;
            sf::Time duration;

            // Constructor
            Transition(const sf::Time &startTime, const sf::Time &duration) :
                startTime(startTime), duration(duration)
            {

            }

            // Methods
            float percent(const sf::Time &currentTime) const;
    };
} // space