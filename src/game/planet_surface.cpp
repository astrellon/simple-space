#include "planet_surface.hpp"

#include "../utils.hpp"

namespace space
{
    PlanetSurface::PlanetSurface(GameSession &session, const PlanetSurfaceDefinition &definition) : _session(session), definition(definition)
    {
        _mapLayer = std::make_unique<MapLayer>(*definition.tmxMap, 0);
        _walkableArea.partOfPlanet(this);
    }

    void PlanetSurface::update(sf::Time dt)
    {
        _mapLayer->update(dt);
        _walkableArea.update(_session, dt, sf::Transform::Identity);
    }

    void PlanetSurface::draw(sf::RenderTarget &target)
    {
        sf::RenderStates states;
        sf::Transform trans;
        trans.scale(Utils::getInsideScale(), Utils::getInsideScale());
        states.transform = trans;

        target.draw(*_mapLayer, states);
        _walkableArea.draw(_session, target);
    }
} // namespace space
