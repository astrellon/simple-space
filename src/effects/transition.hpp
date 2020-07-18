#pragma once

#include <memory>

#include <SFML/System.hpp>

namespace space
{
    class StarSystem;
    class PlanetSurface;

    class Transition
    {
        public:
            // Fields
            StarSystem *fromStarSystem;
            StarSystem *toStarSystem;
            PlanetSurface *fromPlanetSurface;
            PlanetSurface *toPlanetSurface;
            const sf::Time startTime;
            const sf::Time duration;

            // Constructor
            Transition(const sf::Time &startTime, const sf::Time &duration, StarSystem *fromStarSystem, StarSystem *toStarSystem);
            Transition(const sf::Time &startTime, const sf::Time &duration, StarSystem *fromStarSystem, PlanetSurface *toPlanetSurface);
            Transition(const sf::Time &startTime, const sf::Time &duration, PlanetSurface *fromPlanetSurface, StarSystem *toStarSystem);
            Transition(const sf::Time &startTime, const sf::Time &duration, PlanetSurface *fromPlanetSurface, PlanetSurface *toPlanetSurface);

            // Methods
    };
} // space