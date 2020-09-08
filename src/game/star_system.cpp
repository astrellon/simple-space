#include "star_system.hpp"

#include <algorithm>

#include "space_object.hpp"
#include "ship.hpp"
#include "planet.hpp"

#include "../game_session.hpp"
#include "../engine.hpp"
#include "../render_camera.hpp"
#include "../effects/transition.hpp"
#include "../effects/overlay.hpp"
#include "../debug/draw_debug.hpp"
#include "../utils.hpp"

#include "../definitions/planet_definition.hpp"
#include "../definitions/celestial_body_definition.hpp"

namespace space
{
    StarSystem::StarSystem(GameSession &session, const StarSystemDefinition &definition) : definition(definition), _session(session)
    {

    }

    void StarSystem::update(sf::Time dt)
    {
        _background->update(dt);

        for (auto obj : _objects)
        {
            obj->update(_session, dt, sf::Transform::Identity);
        }
    }

    void StarSystem::draw(RenderCamera &target)
    {
        auto insideOfShip = _session.getShipPlayerIsInsideOf();
        auto showInternals = _session.isControllingCharacter();
        if (target.transitionData && target.transitionData->ship)
        {
            insideOfShip = target.transitionData->ship;
            showInternals = true;
        }

        if (!DrawDebug::hideBackground)
        {
            _background->draw(target);
        }

        if (DrawDebug::hideGameSession)
        {
            return;
        }
        target.preDraw();

        for (auto obj : _objects)
        {
            // Ignore rendering current inside ship here
            if (showInternals && obj->id == insideOfShip->id)
            {
                continue;
            }
            obj->draw(_session, target);
        }

        if (showInternals && insideOfShip != nullptr)
        {
            _session.engine().overlay().draw(target, 0.3);
            insideOfShip->draw(_session, target);
        }
    }

    void StarSystem::initFromDefinition()
    {
        createCelestialBody(definition.rootBody.get(), sf::Transform::Identity);
        _background = std::make_unique<StarBackground>(_session.engine(), definition.starBackgroundOptions);
    }

    void StarSystem::addObject(SpaceObject *object)
    {
        if (object == nullptr)
        {
            std::cout << "Cannot add null object to star system" << std::endl;
            return;
        }

        if (object->starSystem() == this)
        {
            return;
        }

        if (object->starSystem() != nullptr)
        {
            object->starSystem()->removeObject(object);
        }

        object->starSystem(this);

        _objects.push_back(object);
    }

    void StarSystem::removeObject(SpaceObject *object)
    {
        if (object == nullptr)
        {
            std::cout << "Cannot remove null object from star system" << std::endl;
            return;
        }

        auto find = std::find(_objects.begin(), _objects.end(), object);
        if (find != _objects.end())
        {
            object->starSystem(nullptr);
            _objects.erase(find);
        }
        else
        {
            std::cout << "Unable to find object: " << object->id << " to remove from star system" << std::endl;
        }
    }

    void StarSystem::getObjectsNearby(float radius, const sf::Vector2f &position, StarSystem::FindObjectCallback callback) const
    {
        auto lengthSquared = radius * radius;
        for (auto obj : _objects)
        {
            auto dist = (position - obj->transform().position).lengthSquared();
            if (dist <= lengthSquared)
            {
                callback(obj);
            }
        }
    }

    void StarSystem::getObjectsNearby(float radius, const sf::Vector2f &position, std::vector<SpaceObject *> &result) const
    {
        auto lengthSquared = radius * radius;
        for (auto obj : _objects)
        {
            auto dist = (position - obj->transform().position).lengthSquared();
            if (dist <= lengthSquared)
            {
                result.push_back(obj);
            }
        }
    }

    void StarSystem::createCelestialBody(const CelestialBodyDefinition *bodyDefinition, sf::Transform parentTransform)
    {
        auto type = bodyDefinition->type();
        if (type == PlanetDefinition::DefinitionType())
        {
            auto planetDefinition = dynamic_cast<const PlanetDefinition *>(bodyDefinition);
            auto planet = _session.createObject<Planet>(planetDefinition->id, *planetDefinition);
            addObject(planet);
        }
        else if (type == OrbitPointCelestialDefinition::DefinitionType())
        {
            // Ignore
        }
        else
        {
            std::cout << "Unknown celestial body definition for star system: " << type << std::endl;
        }

        for (const auto &child : bodyDefinition->children)
        {
            createCelestialBody(child.get(), parentTransform);
        }
    }
} // namespace space
