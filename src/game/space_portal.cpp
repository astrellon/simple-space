#include "space_portal.hpp"

#include <SFML/OpenGL.hpp>

#include "../debug/draw_debug.hpp"
#include "../utils.hpp"
#include "../game_session.hpp"
#include "../engine.hpp"
#include "../render_camera.hpp"
#include "star_system.hpp"
#include "ship.hpp"
#include "planet.hpp"

namespace space
{
    SpacePortal::SpacePortal(const ObjectId &id, const SpacePortalDefinition &definition) :
        SpaceObject(id), definition(definition), _sprite(*definition.texture), _lerpFromShadowT(1.0f)
    {
        _sprite.sequence("idle", true);
        _shadowShape.emplace_back();

        _spriteBounds = _sprite.getGlobalBounds();
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

        _insideArea->getObjectsNearby(definition.pullRadius, pos, [&](SpaceObject *obj)
        {
            if (obj->id == this->id)
            {
                // Teleporting yourself is not allowed!
                return;
            }

            auto ship = dynamic_cast<Ship *>(obj);
            if (ship == nullptr)
            {
                // Only work on ships for now
                return;
            }

            auto shipPoint = ship->transform().position;
            auto portalToShip = shipPoint - pos;
            auto distanceToShip = portalToShip.length();
            auto shipMoveDir = (shipPoint - ship->prevPosition()).normalised();
            auto portalToShipDir = portalToShip / distanceToShip;

            auto &nearby = getNearbyObject(portalToShip, ship);
            nearby.framesOutsideOfRadius = 0;

            auto pp = (nearby.entryP2 - nearby.entryP1).normalised();
            auto cross = pp.cross(portalToShip);
            auto shipToPortalDot = portalToShipDir.dot(shipMoveDir);
            // Make sure we've gone through the middle of the portal edge and that we were moving
            // towards the portal at the same time.
            if (cross < -0.1f && shipToPortalDot > 0.1f)
            {
                auto newPos = targetObject->transform().position + portalToShip; //+ dir * 20.0f;
                auto queue = ship->id == session.playerController().controllingShip()->id;
                session.moveSpaceObject(ship, newPos, targetObject->insideArea(), queue);

                if (targetSpacePortal)
                {
                    auto &otherNearby = targetSpacePortal->getNearbyObject(portalToShip, ship);
                    otherNearby.entryP1 = nearby.entryP2;
                    otherNearby.entryP2 = nearby.entryP1;
                }
            }
        });

        cleanupNearbyObjects();
    }

    void SpacePortal::draw(GameSession &session, RenderCamera &target)
    {
        target.texture().draw(_sprite, _worldTransform);
        DrawDebug::glDraw++;
    }

    bool SpacePortal::doesMouseHover(GameSession &session, sf::Vector2f mousePosition) const
    {
        auto worldPos = Utils::getPosition(_worldTransform);
        auto local = mousePosition - worldPos;
        return _spriteBounds.contains(local);
    }

    bool SpacePortal::isMouseOverPortal(sf::Vector2f mousePosition)
    {
        auto worldPos = Utils::getPosition(_worldTransform);
        auto local = mousePosition - worldPos;
        auto &polygon = _shadowShape[0];

        _mouseOverTriangleIndex = Utils::checkIfInsidePolygon(local, polygon, _earcut);
        return _mouseOverTriangleIndex >= 0;
    }

    void SpacePortal::drawPortal(GameSession &session, RenderCamera &target, bool asPolygon)
    {
        auto pos = Utils::getPosition(_worldTransform);
        auto &camera = session.engine().sceneRender().camera();
        auto playerShip = session.getPlayerShip();
        auto toViewport = camera.center() - pos;
        _shadow.viewPoint = toViewport;
        _shadow.view = camera.view();

        NearPortalObject *nearbyPlayer;
        if (playerShip && tryGetNearbyObject(playerShip, &nearbyPlayer))
        {
            _shadow.point2 = nearbyPlayer->entryP2;
            _shadow.point1 = nearbyPlayer->entryP1;
            _lerpFromShadowT = 0.0f;
            _lerpFromShadowPoint[0] = nearbyPlayer->entryP1;
            _lerpFromShadowPoint[1] = nearbyPlayer->entryP2;
        }
        else
        {
            toViewport = toViewport.normalised();
            auto radius = definition.pullRadius;
            auto point2 = sf::Vector2f(toViewport.y * radius, -toViewport.x * radius);
            auto point1 = sf::Vector2f(-toViewport.y * radius, toViewport.x * radius);

            _shadow.point1 = Utils::paraLerp(_lerpFromShadowPoint[0], point1, _lerpFromShadowT);
            _shadow.point2 = Utils::paraLerp(_lerpFromShadowPoint[1], point2, _lerpFromShadowT);

            _lerpFromShadowT = Utils::clamp01(_lerpFromShadowT + session.engine().deltaTime().asSeconds() * 4.0f);
        }

        _shadowShape[0].clear();
        _shadow.calcShadow(_shadowShape[0], _shadowOutlines);
        _earcut(_shadowShape);

        if (asPolygon)
        {
            sf::VertexArray polygonDraw(sf::Triangles);
            for (auto i = 0; i < _earcut.indices.size(); i++)
            {
                auto index = _earcut.indices[i];
                auto &point = _shadowShape[0][index];
                polygonDraw.append(sf::Vertex(point, sf::Color::White));
            }
            target.texture().draw(polygonDraw, _worldTransform);
        }
        else
        {
            sf::VertexArray polygonDraw(sf::Triangles);
            for (auto i = 0; i < _earcut.indices.size(); i += 3)
            {
                sf::VertexArray polygonDraw(sf::Triangles, 3);

                for (auto j = 0; j < 3; j++)
                {
                    auto index = _earcut.indices[i + j];
                    auto &point = _shadowShape[0][index];

                    auto isOverIndex = (i / 3) == _mouseOverTriangleIndex;
                    polygonDraw[j].position = point;
                    polygonDraw[j].color = isOverIndex ? sf::Color::Red : Utils::hsv((float)i / (float)_earcut.indices.size() * 360, 1, 1);
                    polygonDraw[j].color.a = isOverIndex ? 255 : 120;
                }

                target.texture().draw(polygonDraw, _worldTransform);
            }
        }
    }

    void SpacePortal::drawPortalOutlines(GameSession &session, sf::RenderTarget &target)
    {
        // glLineWidth(3.0f);
        // sf::VertexArray outlineDraw(sf::Lines, 4);
        // outlineDraw.append(sf::Vertex(_shadowOutlines[0].point1, sf::Color::White));
        // outlineDraw.append(sf::Vertex(_shadowOutlines[0].point2, sf::Color::White));
        // outlineDraw.append(sf::Vertex(_shadowOutlines[1].point1, sf::Color::White));
        // outlineDraw.append(sf::Vertex(_shadowOutlines[1].point2, sf::Color::White));

        // target.draw(outlineDraw, _worldTransform);
        // glLineWidth(1.0f);
    }

    void SpacePortal::cleanupNearbyObjects()
    {
        for (auto i = 0; i < _nearbyObjects.size(); i++)
        {
            auto &nearbyObj = _nearbyObjects[i];
            if (nearbyObj.framesOutsideOfRadius > 4)
            {
                _nearbyObjects.erase(_nearbyObjects.begin() + i);
                i--;
            }
            else
            {
                nearbyObj.framesOutsideOfRadius++;
            }
        }
    }

    SpacePortal::NearPortalObject &SpacePortal::getNearbyObject(const sf::Vector2f &portalToShip, Ship *ship)
    {
        for (auto &nearby : _nearbyObjects)
        {
            if (nearby.ship == ship)
            {
                return nearby;
            }
        }

        auto &result = _nearbyObjects.emplace_back(ship);
        result.calcEntryPoints(portalToShip, definition.pullRadius);
        return result;
    }

    bool SpacePortal::tryGetNearbyObject(Ship *ship, SpacePortal::NearPortalObject **result)
    {
        for (auto &nearby : _nearbyObjects)
        {
            if (nearby.ship == ship)
            {
                *result = &nearby;
                return true;
            }
        }

        return false;
    }

    void SpacePortal::NearPortalObject::calcEntryPoints(sf::Vector2f point, float radius)
    {
        point = point.normalised();
        entryP2 = sf::Vector2f(point.y * radius, -point.x * radius);
        entryP1 = sf::Vector2f(-point.y * radius, point.x * radius);
    }

} // space