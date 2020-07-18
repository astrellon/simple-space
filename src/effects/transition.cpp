#include "transition.hpp"

namespace space
{
    Transition::Transition(const sf::Time &startTime, const sf::Time &duration, StarSystem *fromStarSystem, StarSystem *toStarSystem) :
        startTime(startTime), duration(duration),
        fromStarSystem(fromStarSystem), toStarSystem(toStarSystem),
        fromPlanetSurface(nullptr), toPlanetSurface(nullptr)
    {

    }

    Transition::Transition(const sf::Time &startTime, const sf::Time &duration, StarSystem *fromStarSystem, PlanetSurface *toPlanetSurface) :
        startTime(startTime), duration(duration),
        fromStarSystem(fromStarSystem), toStarSystem(nullptr),
        fromPlanetSurface(nullptr), toPlanetSurface(toPlanetSurface)
    {

    }

    Transition::Transition(const sf::Time &startTime, const sf::Time &duration, PlanetSurface *fromPlanetSurface, StarSystem *toStarSystem) :
        startTime(startTime), duration(duration),
        fromStarSystem(nullptr), toStarSystem(toStarSystem),
        fromPlanetSurface(fromPlanetSurface), toPlanetSurface(nullptr)
    {

    }

    Transition::Transition(const sf::Time &startTime, const sf::Time &duration, PlanetSurface *fromPlanetSurface, PlanetSurface *toPlanetSurface) :
        startTime(startTime), duration(duration),
        fromStarSystem(nullptr), toStarSystem(nullptr),
        fromPlanetSurface(fromPlanetSurface), toPlanetSurface(toPlanetSurface)
    {

    }
} // namespace space
