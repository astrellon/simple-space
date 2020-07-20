#pragma once

#include <memory>

#include <SFML/System.hpp>

#include "../game/space_object.hpp"
#include "../camera.hpp"

namespace space
{
    class StarSystem;
    class PlanetSurface;
    class Ship;

    class TransitionData
    {
        public:
            // Fields
            StarSystem *starSystem;
            Ship *ship;
            PlanetSurface *planetSurface;
            sf::Vector2f position;
            float rotation;
            CameraProps cameraProps;

            // Constructor
            TransitionData() : ship(nullptr), starSystem(nullptr), planetSurface(nullptr), rotation(0.0f) { }

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