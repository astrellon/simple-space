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
    StarSystem::StarSystem(const ObjectId &id, const StarSystemDefinition &definition) : SpaceObject(id), definition(definition), _area(AreaType::StarSystem, this)
    {
        _area.main().sortEveryDraw = false;
    }

    StarSystem *StarSystem::cloneStarSystem(const ObjectId &newId, GameSession &session)
    {
        auto result = session.createObject<StarSystem>(newId, definition);
        result->transform(_transform);
        return result;
    }

    void StarSystem::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);

        _background->update(dt);
        _area.update(session, dt, _worldTransform);
    }

    void StarSystem::draw(GameSession &session, RenderCamera &target)
    {
        if (!DrawDebug::hideBackground)
        {
            _background->draw(target);
        }

        if (DrawDebug::hideGameSession)
        {
            return;
        }

        target.preDraw();
        _area.draw(session, target);
    }

    bool StarSystem::checkForMouse(GameSession &session, sf::Vector2f mousePosition)
    {
        return _area.checkForMouse(session, mousePosition);
    }

    void StarSystem::init(GameSession &session)
    {
        createCelestialBody(session, definition.rootBody.get(), sf::Transform::Identity);
        _background = std::make_unique<StarBackground>(session.engine(), definition.starBackgroundOptions);
    }

    void StarSystem::onPostLoad(GameSession &session, LoadingContext &context)
    {
        _area.onPostLoad(session, context);
    }

    void StarSystem::createCelestialBody(GameSession &session, const CelestialBodyDefinition *bodyDefinition, const sf::Transform &parentTransform)
    {
        auto type = bodyDefinition->type();
        if (type == PlanetDefinition::DefinitionType())
        {
            auto planetDefinition = dynamic_cast<const PlanetDefinition *>(bodyDefinition);

            std::stringstream idss;
            idss << id << ':' << planetDefinition->id;

            auto planet = session.createObject<Planet>(idss.str(), *planetDefinition);
            _area.addObject(planet);
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
            createCelestialBody(session, child.get(), parentTransform);
        }
    }
} // namespace space
