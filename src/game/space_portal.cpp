#include "space_portal.hpp"

#include <SFML/OpenGL.hpp>

#include "../debug/draw_debug.hpp"
#include "../utils.hpp"
#include "../game_session.hpp"
#include "star_system.hpp"
#include "ship.hpp"
#include "planet.hpp"
#include "../engine.hpp"

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

        auto targetSpacePortal = dynamic_cast<SpacePortal *>(targetObject);

        _sprite.sequence("active", false);
        auto seconds = dt.asSeconds();

        auto pos = Utils::getPosition(_worldTransform);

        _insideStarSystem->getObjectsNearby(definition.pullRadius, pos, [&](SpaceObject *obj)
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

            auto isIdToIgnore = foundId(obj->id);

            auto portalToShip = pos - obj->transform().position;
            auto distanceToShip = portalToShip.length();
            auto dir = portalToShip / distanceToShip;
            if (distanceToShip < 10)
            {
                auto newPos = targetObject->transform().position + dir * 20.0f;
                auto queue = obj->id == session.playerController().controllingShip()->id;
                session.moveSpaceObject(obj, newPos, targetObject->starSystem(), queue);

                if (targetSpacePortal)
                {
                    targetSpacePortal->ignoreId(obj->id);
                }
                return;
            }

            if (isIdToIgnore)
            {
                return;
            }

            auto ship = dynamic_cast<Ship *>(obj);
            if (ship == nullptr)
            {
                return;
            }

            auto force = Utils::clamp((definition.pullRadius - distanceToShip) / definition.pullRadius, 0, 1.0f);
            auto speedAddition = dir * force * definition.pullForce * seconds;

            ship->speed(ship->speed() + speedAddition);
        });

        for (auto i = 0; i < _idsToIgnore.size(); i++)
        {
            auto &idToIgnore = _idsToIgnore[i];
            if (idToIgnore.framesOutsideOfRadius > 10)
            {
                _idsToIgnore.erase(_idsToIgnore.begin() + i);
                i--;
            }
            else
            {
                idToIgnore.framesOutsideOfRadius++;
            }
        }
    }

    void SpacePortal::draw(GameSession &session, sf::RenderTarget &target)
    {
        target.draw(_sprite, _worldTransform);
        DrawDebug::glDraw++;

        auto pos = Utils::getPosition(_worldTransform);
        auto toViewport = session.engine().sceneRender().camera().center() - pos;
        _shadow.viewPoint = toViewport;
        _shadow.view = session.engine().sceneRender().camera().view();

        toViewport = toViewport.normalised();
        _shadow.point2 = sf::Vector2f(toViewport.y * 20.0f, -toViewport.x * 20.0f);
        _shadow.point1 = sf::Vector2f(-toViewport.y * 20.0f, toViewport.x * 20.0f);

        auto polygon = _shadow.calcShadow();
        sf::VertexArray polygonDraw(sf::LineStrip);
        glLineWidth(4.0f);
        for (auto &point : polygon)
        {
            polygonDraw.append(sf::Vertex(point, sf::Color::White));
        }
        target.draw(polygonDraw, _worldTransform);
        glLineWidth(1.0f);
    }

    void SpacePortal::ignoreId(const ObjectId &id)
    {
        _idsToIgnore.emplace_back(id);
    }

    bool SpacePortal::foundId(const ObjectId &id)
    {
        for (auto &idToIgnore : _idsToIgnore)
        {
            if (idToIgnore.id == id)
            {
                idToIgnore.framesOutsideOfRadius = 0;
                return true;
            }
        }

        return false;
    }

} // space