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
    CharacterController::CharacterController(GameSession &session) :
        _session(session),
        _ship(nullptr),
        _character(nullptr),
        _teleportClone(nullptr),
        _controlling(ControlNone),
        _inventory(std::make_unique<Inventory>()),
        _inIdleAnimation(true),
        _interactRangeObjects(0),
        _interactRangeShips(0),
        _timeToNextIdle(0.0f),
        _dizzy(0.0f)
    {
        interactRangeObjects(100.0f);
        interactRangeShips(150.0f);
    }

    void CharacterController::dropItem(PlaceableItem *placeableItem)
    {
        _inventory->removeItem(placeableItem);
        _character->insideArea()->addPlaceable(_session, placeableItem, _character->transform().position);
    }

    SpaceObject *CharacterController::controllingObject() const
    {
        if (_controlling == ControlShip)
        {
            return _ship;
        }
        if (_controlling == ControlCharacter)
        {
            return _character;
        }

        return nullptr;
    }

    void CharacterController::checkForInteractables(sf::Vector2f position, const Area &area)
    {
        auto playerPos = _character->transform().position;
        auto rangeSquared = _interactRangeObjects * _interactRangeObjects;
        auto outOfRange = rangeSquared + 1.0f;
        auto size = sf::Vector2f(_interactRangeObjects, _interactRangeObjects);
        //_canInteractWithInRange.clear();
        std::vector<Interactable *> interactablesToRemove = _canInteractWithInRange.interactables();

        auto thisInteractable = _character ? _character->interactable() : nullptr;
        auto roughArea = sf::FloatRect(playerPos - size * 0.5f, size);
        for (auto obj : area.getNearbyObjects(roughArea))
        {
            auto interactable = obj->interactable();
            if (!interactable || interactable == thisInteractable || interactable->interactions().size() == 0)
            {
                continue;
            }

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
                interactable->onCharacterEnters(this, _session);
            }
            else if (prevDist <= rangeSquared && distance > rangeSquared)
            {
                interactable->onCharacterLeaves(this, _session);
            }

            if (distance <= rangeSquared)
            {
                _canInteractWithInRange.addInteractable(interactable);
                Utils::remove(interactablesToRemove, interactable);
            }

            _canInteractWith[interactable] = distance;
        }

        for (auto interactable : interactablesToRemove)
        {
            _canInteractWithInRange.removeInteractable(interactable);
        }
    }

    void CharacterController::checkForInTeleportRange(sf::Vector2f position, const Area &area)
    {
        static std::vector<SpaceObject *> objs;
        objs.clear();
        area.getObjectsNearby(_interactRangeShips, position, objs);

        for (auto obj : objs)
        {
            auto shipInsideOf = _character->insideArea()->partOfShip();

            if (obj == shipInsideOf || obj == _character)
            {
                return;
            }

            Ship *ship;
            if (obj->tryCast(ship))
            {
                ship->area().addTeleporters(_teleportersInRange);
                continue;
            }

            Planet *planet;
            if (obj->tryCast(planet))
            {
                for (auto planetSurface : planet->planetSurfaces())
                {
                    planetSurface->area().addTeleporters(_teleportersInRange);
                }
            }
        }
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

        if (!_inIdleAnimation)
        {
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
            if (sprite.hasSequence("idle2"))
            {
                if (Utils::randi(0, 100) < 40)
                {
                    sprite.sequence("idle2", immediateChange);
                }
                else
                {
                    sprite.sequence("idle", immediateChange);
                }
            }
            else
            {
                sprite.sequence("idle", immediateChange);
            }

            _timeToNextIdle = Utils::randf(2, 5);
        }
    }
} // namespace space
