#include "planet_surface.hpp"

#include "../utils.hpp"
#include "../render_camera.hpp"

namespace space
{
    PlanetSurface::PlanetSurface(GameSession &session, const PlanetSurfaceDefinition &definition) : _session(session), definition(definition), _partOfPlanet(nullptr)
    {
        _mapLayer = std::make_unique<MapLayer>(*definition.tmxMap, 0);
        _walkableArea.partOfPlanetSurface(this);
    }

    void PlanetSurface::update(sf::Time dt)
    {
        _mapLayer->update(dt);
        _walkableArea.update(_session, dt, sf::Transform::Identity);
    }

    void PlanetSurface::draw(RenderCamera &target)
    {
        target.preDraw();

        sf::RenderStates states;
        sf::Transform trans;
        trans.scale(Utils::getInsideScale(), Utils::getInsideScale());
        states.transform = trans;

        target.texture().clear(sf::Color(60, 90, 70));

        target.texture().draw(*_mapLayer, states);
        _walkableArea.draw(_session, target);
    }
} // namespace space