#include "space_portal.hpp"

#include "../debug/draw_debug.hpp"
#include "../utils.hpp"
#include "../game_session.hpp"
#include "star_system.hpp"
#include "ship.hpp"
#include "planet.hpp"

namespace space
{
    SpacePortal::SpacePortal(const ObjectId &id, const SpacePortalDefinition &definition) :
        SpaceObject(id), definition(definition), _sprite(*definition.texture)
    {
        _sprite.sequence("idle", true);
    }

    void SpacePortal::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);

        _sprite.update(dt);

        if (targetObjectId.size() == 0)
        {
            _sprite.sequence("idle", false);
            return;
        }

        SpaceObject *targetObject;
        if (!session.tryGetSpaceObject(targetObjectId, &targetObject))
        {
            std::cout << "Unable to find target object for space portal" << std::endl;
            _sprite.sequence("idle", false);
            return;
        }

        _sprite.sequence("active", false);
        auto seconds = dt.asSeconds();

        auto pos = Utils::getPosition(_worldTransform);
        _insideStarSystem->getObjectsNearby(50.0f, pos, [&](SpaceObject *obj)
        {
            if (obj->id == this->id)
            {
                // Teleporting yourself is not allowed!
                return;
            }

            auto planet = dynamic_cast<Planet *>(obj);
            if (planet != nullptr)
            {
                // No sucking up planets right now.
                return;
            }

            auto portalToShip = pos - obj->transform().position;
            auto distanceToShip = Utils::length(portalToShip);
            auto dir = portalToShip / distanceToShip;
            if (distanceToShip < 10)
            {
                auto newPos = targetObject->transform().position + dir * 20.0f;
                auto queue = obj->id == session.playerController().controllingShip()->id;
                session.moveSpaceObject(obj, newPos, targetObject->starSystem(), queue);
                return;
            }

            // auto ship = dynamic_cast<Ship *>(obj);
            // if (ship == nullptr)
            // {
            //     return;
            // }

            // auto force = Utils::clamp(50.0f - distanceToShip, 0, 20.0f);
            // ship->speed(ship->speed() + dir * force * seconds);
        });
    }

    void SpacePortal::draw(GameSession &session, sf::RenderTarget &target)
    {
        target.draw(_sprite, _worldTransform);
        DrawDebug::glDraw++;
    }

} // space