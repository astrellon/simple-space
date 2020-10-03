#include "planet_surface.hpp"

#include "../utils.hpp"
#include "../render_camera.hpp"
#include "../game_session.hpp"
#include "../engine.hpp"
#include "walkable_area_instances.hpp"

namespace space
{
    PlanetSurface::PlanetSurface(GameSession &session, const PlanetSurfaceDefinition &definition) : PlanetSurface(session, definition, std::make_unique<WalkableArea>())
    {

    }

    PlanetSurface::PlanetSurface(GameSession &session, const PlanetSurfaceDefinition &definition, std::unique_ptr<WalkableArea> walkableArea) : _session(session), definition(definition), _partOfPlanet(nullptr), _walkableArea(std::move(walkableArea))
    {
        const auto &layers = definition.tmxMap->getLayers();
        for (auto i = 0; i < layers.size(); i++)
        {
            _mapLayers.emplace_back(std::make_unique<MapLayer>(*definition.tmxMap, session.engine().resourceManager(), i));
        }
        _walkableArea->partOfPlanetSurface(this);
    }

    void PlanetSurface::update(sf::Time dt)
    {
        for (auto &mapLayer : _mapLayers)
        {
            mapLayer->update(dt);
        }

        _walkableArea->update(_session, dt, sf::Transform::Identity);
    }

    void PlanetSurface::draw(RenderCamera &target)
    {
        target.preDraw();

        sf::RenderStates states;
        sf::Transform trans;
        trans.scale(Utils::getInsideScale(), Utils::getInsideScale());
        states.transform = trans;

        target.texture().clear(sf::Color(60, 90, 70));

        for (auto &mapLayer : _mapLayers)
        {
            target.texture().draw(*mapLayer, states);
        }
        _walkableArea->draw(_session, target);
    }

    void PlanetSurface::onPostLoad(LoadingContext &context)
    {
        _walkableArea->onPostLoad(_session, context);
        definition.walkableAreaInstances.applyToWalkableArea(*_walkableArea, _session);
    }

    void PlanetSurface::checkForMouse(sf::Vector2f mousePosition)
    {
        _walkableArea->checkForMouse(_session, mousePosition);
    }
} // namespace space
