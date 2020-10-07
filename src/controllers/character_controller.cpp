#include "character_controller.hpp"

#include <SFML/Window.hpp>

#include "../game/ship.hpp"
#include "../game/character.hpp"
#include "../game/planet.hpp"
#include "../game/planet_surface.hpp"
#include "../game/items/item.hpp"
#include "../game/items/placeable_item.hpp"
#include "../game/area.hpp"
#include "../game/star_system.hpp"
#include "../utils.hpp"

namespace space
{
    CharacterController::CharacterController(GameSession &session) : _session(session), _controlling(ControlNone), _inventory(std::make_unique<Inventory>()),
    _interactRangeObjects(0), _interactRangeShips(0),
    _character(nullptr), _ship(nullptr), _teleportClone(nullptr), _timeToNextIdle(0.0f), _dizzy(0.0f)
    {
        interactRangeObjects(10.0f);
        interactRangeShips(150.0f);
    }

    void CharacterController::dropItem(PlaceableItem *placeableItem)
    {
        _inventory->removeItem(placeableItem);
        //_character->insideArea()->addPlaceable(_session, placeableItem, _character->transform().position);
    }

    void CharacterController::checkForInteractables(sf::Vector2f position, const Area &area)
    {
        auto playerPos = _character->transform().position;
        auto rangeSquared = _interactRangeObjects * _interactRangeObjects;
        auto outOfRange = rangeSquared + 1.0f;
        _canInteractWithInRange.clear();

        for (auto obj : area.objects())
        {
            auto interactable = &obj->interactable();

            auto find = _canInteractWith.find(interactable);
            auto dpos = interactable->parentObject()->transform().position - playerPos;
            auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
            auto prevDist = outOfRange;

            if (find != _canInteractWith.end())
            {
                prevDist = find->second;
            }

            if (prevDist > rangeSquared && distance <= rangeSquared)
            {
                interactable->onPlayerEnters(_session);
            }
            else if (prevDist <= rangeSquared && distance > rangeSquared)
            {
                interactable->onPlayerLeaves(_session);
            }

            if (distance <= rangeSquared)
            {
                _canInteractWithInRange.push_back(interactable);
            }

            _canInteractWith[interactable] = distance;
        }
    }

    void CharacterController::checkForInTeleportRange(sf::Vector2f position, const Area &area, std::vector<PlacedItemPair<Teleporter>> &teleportersInRange)
    {
        auto shipInsideOf = _character->insideArea()->partOfShip();
        auto character = _character;

        area.getObjectsNearby(_interactRangeShips, position, [&](SpaceObject *obj)
        {
            if (obj == shipInsideOf || obj == character)
            {
                return;
            }

            auto type = obj->type();
            if (type == Ship::SpaceObjectType())
            {
                auto ship = dynamic_cast<Ship *>(obj);
                ship->area().addTeleporters(teleportersInRange);
            }
            else if (type == Planet::SpaceObjectType())
            {
                auto planet = dynamic_cast<Planet *>(obj);
                for (auto planetSurface : planet->planetSurfaces())
                {
                    planetSurface->area().addTeleporters(teleportersInRange);
                }
            }
        });
    }

    void CharacterController::clearCanInteractWith()
    {
        _canInteractWith.clear();
        _canInteractWithInRange.clear();
    }

    void CharacterController::updateAnimations(sf::Time dt)
    {
        auto seconds = dt.asSeconds();
        auto rotSpeed = 0.0f;
        if (_character->physicsBody())
        {
            rotSpeed = std::abs(_character->physicsBody()->GetAngularVelocity());
        }
        _dizzy += (rotSpeed - 8.0f) * seconds;
        _dizzy = Utils::clamp(_dizzy, 0, 5.0f);

        auto &sprite = _character->sprite();

        if (_dizzy > 1.0f)
        {
            if (sprite.currentAnimation() != "dizzy")
            {
                sprite.sequence("dizzy", true);
                _timeToNextIdle = 1.0f;
            }
            return;
        }

        auto immediateChange = false;
        if (_character->moveInput != sf::Vector2f() && !_character->isInSpace())
        {
            sprite.sequence("walk", true);
            _timeToNextIdle = 0;

            if (_character->moveInput.x < 0)
            {
                _character->flipSprite(true);
            }
            else if (_character->moveInput.x > 0)
            {
                _character->flipSprite(false);
            }

            return;
        }
        else if (sprite.currentAnimation() == "walk")
        {
            immediateChange = true;
        }

        _timeToNextIdle -= dt.asSeconds();
        if (_timeToNextIdle < 0)
        {
            if (Utils::randi(0, 100) < 40)
            {
                sprite.sequence("idle2", immediateChange);
            }
            else
            {
                sprite.sequence("idle", immediateChange);
            }

            _timeToNextIdle = Utils::randf(2, 5);
        }
    }
} // namespace space
