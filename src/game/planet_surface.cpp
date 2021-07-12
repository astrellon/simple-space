#include "planet_surface.hpp"

#include "../utils.hpp"
#include "../render_camera.hpp"
#include "../game_session.hpp"
#include "../engine.hpp"
#include "area_instances.hpp"

namespace space
{
    const SpaceObjectType PlanetSurface::TypeValue = SpaceObjectType::PlanetSurface;

    PlanetSurface::PlanetSurface(const ObjectId &id, const PlanetSurfaceDefinition &definition) : SpaceObject(id), definition(definition), _area(AreaType::PlanetSurface, this), _partOfPlanet(nullptr)
    {
    }

    SpaceObject *PlanetSurface::deepClone(const ObjectId &newIdPrefix, const CloneContext &context)
    {
        auto result = clonePlanetSurface(newIdPrefix + id, context);

        if (Utils::contains(context.showingAreas, &_area))
        {
            _area.cloneInto(newIdPrefix, result->area(), context);
        }

        return result;
    }

    PlanetSurface *PlanetSurface::clonePlanetSurface(const ObjectId &newId, const CloneContext &context)
    {
        std::cout << "Clone definition: " << definition.name << std::endl;
        auto result = context.session.createObject<PlanetSurface>(newId, definition);
        result->createMapLayersFromDefinition(context.session);
        populateCloneFromThis(result, context);
        result->partOfPlanet(_partOfPlanet);
        return result;
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
        trans.scale(Utils::InsideScale, Utils::InsideScale);
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
        SpaceObject::onPostLoad(session, context);
        createMapLayersFromDefinition(session);

        _area.onPostLoad(session, context);
        //definition.walkableAreaInstances.applyToWalkableArea(*_walkableArea, _session);

        if (!_partOfLivePhoto)
        {
            session.addToUpdateEveryFrame(this);
        }
    }

    bool PlanetSurface::checkForMouse(const Area *inRelationTo, GameSession &session, sf::Vector2f mousePosition)
    {
        return _area.checkForMouse(inRelationTo, session, mousePosition);
    }

    bool PlanetSurface::loopOver(LoopObjectCallback callback)
    {
        if (!SpaceObject::loopOver(callback))
        {
            return false;
        }
        if (!_area.loopOver(callback))
        {
            return false;
        }
        return true;
    }

    void PlanetSurface::createMapLayersFromDefinition(GameSession &session)
    {
        const auto &layers = definition.tmxMap->getLayers();
        for (auto i = 0u; i < layers.size(); i++)
        {
            _mapLayers.emplace_back(std::make_unique<MapLayer>(*definition.tmxMap, session.engine().resourceManager(), i));
        }
    }
} // namespace space
