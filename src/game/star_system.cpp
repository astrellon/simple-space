#include "star_system.hpp"

#include <algorithm>

#include "space_object.hpp"
#include "game_session.hpp"
#include "planet.hpp"
#include "../definitions/planet_definition.hpp"
#include "../definitions/celestial_body_definition.hpp"

namespace space
{
    StarSystem::StarSystem(GameSession &session, const StarSystemDefinition &definition) : definition(definition), _session(session)
    {

    }

    void StarSystem::update(Engine &engine, sf::Time dt)
    {
    }

    void StarSystem::draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        for (auto obj : _objects)
        {
            obj->draw(engine, target, parentTransform);
        }
    }

    void StarSystem::initFromDefinition()
    {
        createCelestialBody(definition.rootBody.get());
    }

    void StarSystem::addObject(SpaceObject *object)
    {
        if (object == nullptr)
        {
            std::cout << "Cannot add null object to star system" << std::endl;
            return;
        }
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
} // namespace space
