#include "star_system.hpp"

#include <algorithm>

#include "space_object.hpp"
#include "ship.hpp"
#include "game_session.hpp"
#include "planet.hpp"
#include "engine.hpp"

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

        auto ship = _session.getShipPlayerIsInsideOf();
        if (ship != nullptr && ship->starSystem() == this)
        {
            checkForTeleportableShips();
        }
    }

    void StarSystem::draw(sf::RenderTarget &target)
    {
        auto showInternals = _session.isControllingCharacter();
        auto insideOfShip = _session.getShipPlayerIsInsideOf();

        _background->draw(target);

        target.setView(_session.engine().camera().view());

        for (auto obj : _objects)
        {
            // Ignore rendering current inside ship here
            if (showInternals && dynamic_cast<Ship *>(obj) == insideOfShip)
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
        createCelestialBody(definition.rootBody.get());
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

    void StarSystem::createCelestialBody(const CelestialBodyDefinition *bodyDefinition)
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
            createCelestialBody(child.get());
        }
    }

    void StarSystem::checkForTeleportableShips()
    {
        auto &player = _session.playerController();
        auto shipInsideOf = _session.getShipPlayerIsInsideOf();

        // Check existing items
        auto playerPos = player.controllingShip()->transform().position;
        for (auto ship : player.shipsInTeleportRange())
        {
            if (ship == shipInsideOf)
            {
                player.removeShipInTeleportRange(ship);
                continue;
            }

            auto dpos = ship->transform().position - playerPos;
            auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
            if (distance - player.interactRangeShipsSquared() > 0.0f)
            {
                player.removeShipInTeleportRange(ship);
            }
        }

        for (auto obj : _objects)
        {
            auto ship = dynamic_cast<Ship *>(obj);
            if (ship == nullptr || ship == shipInsideOf || player.shipInTeleportRange(ship))
            {
                continue;
            }

            auto dpos = ship->transform().position - playerPos;
            auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
            if (distance - player.interactRangeShipsSquared() < 0.0f)
            {
                player.addShipInTeleportRange(ship);
            }
        }
    }
} // namespace space
