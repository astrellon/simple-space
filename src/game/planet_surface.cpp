#include "planet_surface.hpp"

#include "../utils.hpp"
#include "../render_camera.hpp"
#include "../game_session.hpp"
#include "../engine.hpp"
#include "walkable_area_instances.hpp"

namespace space
{
    PlanetSurface::PlanetSurface(const PlanetSurfaceDefinition &definition) : SpaceObject(definition.id), definition(definition), _partOfPlanet(nullptr), _area(true, this)
    {
    }

    void PlanetSurface::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransforms)
    {
        for (auto &mapLayer : _mapLayers)
        {
            mapLayer->update(dt);
        }

        _area.update(session, dt, parentTransforms);
    }

    void PlanetSurface::draw(GameSession &session, RenderCamera &target)
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
        _area.draw(session, target);
    }

    void PlanetSurface::onPostLoad(GameSession &session, LoadingContext &context)
    {
        const auto &layers = definition.tmxMap->getLayers();
        for (auto i = 0; i < layers.size(); i++)
        {
            _mapLayers.emplace_back(std::make_unique<MapLayer>(*definition.tmxMap, session.engine().resourceManager(), i));
        }

        _area.onPostLoad(session, context);
        //definition.walkableAreaInstances.applyToWalkableArea(*_walkableArea, _session);
    }

    bool PlanetSurface::checkForMouse(GameSession &session, sf::Vector2f mousePosition)
    {
        return _area.checkForMouse(session, mousePosition);
    }
} // namespace space
