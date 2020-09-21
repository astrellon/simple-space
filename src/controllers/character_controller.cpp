#include "character_controller.hpp"

#include <SFML/Window.hpp>

#include "../game/ship.hpp"
#include "../game/character.hpp"
#include "../game/planet.hpp"
#include "../game/planet_surface.hpp"
#include "../game/items/item.hpp"
#include "../game/items/placeable_item.hpp"
#include "../game/walkable_area.hpp"
#include "../game/star_system.hpp"
#include "../utils.hpp"

namespace space
{
    CharacterController::CharacterController(GameSession &session) : _session(session), _controlling(ControlNone), _inventory(std::make_unique<Inventory>()),
    _interactRangeObjects(0), _interactRangeObjectsSquared(0),
    _interactRangeShips(0), _interactRangeShipsSquared(0),
    _character(nullptr), _ship(nullptr), _teleportClone(nullptr), _timeToNextIdle(0.0f), _dizzy(0.0f)
    {
        interactRangeObjects(10.0f);
        interactRangeShips(150.0f);
    }

    bool CharacterController::addCanInteractWith(Interactable *item)
    {
        auto find = std::find(_canInteractWith.begin(), _canInteractWith.end(), item);
        if (find == _canInteractWith.end())
        {
            _canInteractWith.push_back(item);
            return true;
        }

        return false;
    }
    bool CharacterController::removeCanInteractWith(Interactable *item)
    {
        auto find = std::find(_canInteractWith.begin(), _canInteractWith.end(), item);
        if (find != _canInteractWith.end())
        {
            _canInteractWith.erase(find);
            return true;
        }

        return false;
    }
    bool CharacterController::canInteractWith(Interactable *item) const
    {
        auto find = std::find(_canInteractWith.begin(), _canInteractWith.end(), item);
        return find != _canInteractWith.end();
    }

    void CharacterController::addShipInTeleportRange(Ship *ship)
    {
        _shipsInTeleportRange.push_back(ship);
    }
    void CharacterController::removeShipInTeleportRange(Ship *ship)
    {
        auto find = std::find(_shipsInTeleportRange.begin(), _shipsInTeleportRange.end(), ship);
        if (find != _shipsInTeleportRange.end())
        {
            _shipsInTeleportRange.erase(find);
        }
    }
    bool CharacterController::shipInTeleportRange(Ship *ship) const
    {
        auto find = std::find(_shipsInTeleportRange.begin(), _shipsInTeleportRange.end(), ship);
        return find != _shipsInTeleportRange.end();
    }

    void CharacterController::addPlanetInTeleportRange(Planet *planet)
    {
        _planetsInTeleportRange.push_back(planet);
    }
    void CharacterController::removePlanetInTeleportRange(Planet *planet)
    {
        auto find = std::find(_planetsInTeleportRange.begin(), _planetsInTeleportRange.end(), planet);
        if (find != _planetsInTeleportRange.end())
        {
            _planetsInTeleportRange.erase(find);
        }
    }
    bool CharacterController::planetInTeleportRange(Planet *planet) const
    {
        auto find = std::find(_planetsInTeleportRange.begin(), _planetsInTeleportRange.end(), planet);
        return find != _planetsInTeleportRange.end();
    }

    void CharacterController::dropItem(PlaceableItem *placeableItem)
    {
        _inventory->removeItem(placeableItem);
        _character->insideArea()->addPlaceable(placeableItem, _character->transform().position);
    }

    void CharacterController::checkForTeleportableShips(sf::Vector2f position, const StarSystem &starSystem)
    {
        auto shipInsideOf = _character->insideArea()->partOfShip();

        // Check existing items
        for (auto ship : _shipsInTeleportRange)
        {
            if (ship == shipInsideOf)
            {
                removeShipInTeleportRange(ship);
                continue;
            }

            auto dpos = ship->transform().position - position;
            auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
            if (distance - interactRangeShipsSquared() > 0.0f)
            {
                removeShipInTeleportRange(ship);
            }
        }

        for (auto obj : starSystem.objects())
        {
            auto ship = dynamic_cast<Ship *>(obj);
            if (ship == nullptr || ship == shipInsideOf || shipInTeleportRange(ship))
            {
                continue;
            }

            auto dpos = ship->transform().position - position;
            auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
            if (distance - interactRangeShipsSquared() < 0.0f)
            {
                addShipInTeleportRange(ship);
            }
        }
    }

    void CharacterController::checkForTeleportablePlanets(sf::Vector2f position, const StarSystem &starSystem)
    {
        // Check existing items
        for (auto planet : _planetsInTeleportRange)
        {
            auto dpos = planet->transform().position - position;
            auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
            if (distance - interactRangeShipsSquared() > 0.0f)
            {
                removePlanetInTeleportRange(planet);
            }
        }

        for (auto obj : starSystem.objects())
        {
            auto planet = dynamic_cast<Planet *>(obj);
            if (planet == nullptr || planetInTeleportRange(planet))
            {
                continue;
            }

            auto dpos = planet->transform().position - position;
            auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
            if (distance - interactRangeShipsSquared() < 0.0f)
            {
                addPlanetInTeleportRange(planet);
            }
        }
    }

    void CharacterController::checkForGroundInteractables(sf::Vector2f position, const WalkableArea &area)
    {
        // Check existing items
        auto playerPos = _character->transform().position;
        for (auto interactable : canInteractWith())
        {
            auto dpos = interactable->parentObject()->transform().position - playerPos;
            auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
            if (distance - interactRangeObjectsSquared() > 0.0f)
            {
                removeCanInteractWith(interactable);
                if (interactable->isPlayerInRange())
                {
                    interactable->onPlayerLeaves(_session);
                }
            }
        }

        for (auto &iter : area.placedItems())
        {
            checkInRangeOfInteractable(&iter->interactable());
        }

        for (auto &iter : area.characters())
        {
            if (iter == _character)
            {
                continue;
            }
            checkInRangeOfInteractable(&iter->interactable());
        }
    }

    void CharacterController::checkInRangeOfInteractable(Interactable *interactable)
    {
        if (canInteractWith(interactable))
        {
            return;
        }

        auto playerPos = _character->transform().position;
        auto dpos = interactable->parentObject()->transform().position - playerPos;
        auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
        if (distance - interactRangeObjectsSquared() < 0.0f)
        {
            if (addCanInteractWith(interactable))
            {
                if (!interactable->isPlayerInRange())
                {
                    interactable->onPlayerEnters(_session);
                }
            }
        }
    }

    void CharacterController::updateAnimations(sf::Time dt)
    {
        auto seconds = dt.asSeconds();
        auto rotSpeed = std::abs(_character->physicsBody()->GetAngularVelocity());
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
        if (_character->moveInput != sf::Vector2f())
        {
            sprite.sequence("walk", true);
            _timeToNextIdle = 0;

            if (!_character->isInSpace())
            {
                if (_character->moveInput.x < 0)
                {
                    _character->flipSprite(true);
                }
                else if (_character->moveInput.x > 0)
                {
                    _character->flipSprite(false);
                }
            }
            else
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
