#include "player_controller.hpp"

#include <SFML/Window.hpp>

#include "game/ship.hpp"
#include "game/character.hpp"
#include "game/planet.hpp"
#include "game/planet_surface.hpp"
#include "keyboard.hpp"

#include "game/items/item.hpp"
#include "game/items/placeable_item.hpp"
#include "game/walkable_area.hpp"
#include "game/star_system.hpp"

namespace space
{
    PlayerController::PlayerController(GameSession &session) : _session(session), _controlling(ControlNone), _inventory(),
    _interactRangeObjects(0), _interactRangeObjectsSquared(0),
    _interactRangeShips(0), _interactRangeShipsSquared(0)
    {
        interactRangeObjects(10.0f);
        interactRangeShips(150.0f);
    }

    void PlayerController::update(sf::Time dt)
    {
        if (_controlling == ControlNone)
        {
            return;
        }

        if (_character && _character->insideArea())
        {
            auto area = _character->insideArea();
            auto planetSurface = area->partOfPlanetSurface();
            if (planetSurface != nullptr && planetSurface->partOfPlanet() != nullptr)
            {
                auto onPlanet = area->partOfPlanetSurface()->partOfPlanet();
                checkForTeleportableShips(onPlanet->transform().position, *onPlanet->starSystem());
                checkForTeleportablePlanets(onPlanet->transform().position, *onPlanet->starSystem());
            }
            else if (area->partOfShip() != nullptr)
            {
                auto insideShip = area->partOfShip();
                checkForTeleportableShips(insideShip->transform().position, *insideShip->starSystem());
                checkForTeleportablePlanets(insideShip->transform().position, *insideShip->starSystem());
            }
        }
        else
        {
            clearPlanetsInTeleportRange();
            clearShipsInTeleportRange();
        }

        if (_controlling == ControlShip)
        {
            controlShip(dt);
        }
        else if (_controlling == ControlCharacter)
        {
            if (_character && _character->insideArea())
            {
                checkForGroundInteractables(_character->transform().position, *_character->insideArea());
            }
            else
            {
                clearCanInteractWith();
            }
            controlCharacter(dt);
        }
    }

    void PlayerController::controlShip(sf::Time dt)
    {
        sf::Vector2f moveInput;
        float rotateInput = 0;

        if (Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            moveInput.x -= 1;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            moveInput.x += 1;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            moveInput.y -= 1;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            moveInput.y += 1;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            rotateInput -= 1;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            rotateInput += 1;
        }

        _ship->moveInput = moveInput;
        _ship->rotateInput = rotateInput;
    }

    void PlayerController::controlCharacter(sf::Time dt)
    {
        sf::Vector2f moveInput;
        float rotateInput = 0;

        if (Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            moveInput.x -= 1;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            moveInput.x += 1;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            moveInput.y -= 1;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            moveInput.y += 1;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            rotateInput -= 1;
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            rotateInput += 1;
        }
        // if (Keyboard::isKeyPressed(sf::Keyboard::F))
        // {
        //     interactWithClosest();
        // }

        _character->moveInput = moveInput;
        _character->rotateInput = rotateInput;
    }

    bool PlayerController::addCanInteractWith(PlacedItem *item)
    {
        auto find = std::find(_canInteractWith.begin(), _canInteractWith.end(), item);
        if (find == _canInteractWith.end())
        {
            _canInteractWith.push_back(item);
            return true;
        }

        return false;
    }
    bool PlayerController::removeCanInteractWith(PlacedItem *item)
    {
        auto find = std::find(_canInteractWith.begin(), _canInteractWith.end(), item);
        if (find != _canInteractWith.end())
        {
            _canInteractWith.erase(find);
            return true;
        }

        return false;
    }
    bool PlayerController::canInteractWith(PlacedItem *item) const
    {
        auto find = std::find(_canInteractWith.begin(), _canInteractWith.end(), item);
        return find != _canInteractWith.end();
    }

    void PlayerController::addShipInTeleportRange(Ship *ship)
    {
        _shipsInTeleportRange.push_back(ship);
    }
    void PlayerController::removeShipInTeleportRange(Ship *ship)
    {
        auto find = std::find(_shipsInTeleportRange.begin(), _shipsInTeleportRange.end(), ship);
        if (find != _shipsInTeleportRange.end())
        {
            _shipsInTeleportRange.erase(find);
        }
    }
    bool PlayerController::shipInTeleportRange(Ship *ship) const
    {
        auto find = std::find(_shipsInTeleportRange.begin(), _shipsInTeleportRange.end(), ship);
        return find != _shipsInTeleportRange.end();
    }

    void PlayerController::addPlanetInTeleportRange(Planet *planet)
    {
        _planetsInTeleportRange.push_back(planet);
    }
    void PlayerController::removePlanetInTeleportRange(Planet *planet)
    {
        auto find = std::find(_planetsInTeleportRange.begin(), _planetsInTeleportRange.end(), planet);
        if (find != _planetsInTeleportRange.end())
        {
            _planetsInTeleportRange.erase(find);
        }
    }
    bool PlayerController::planetInTeleportRange(Planet *planet) const
    {
        auto find = std::find(_planetsInTeleportRange.begin(), _planetsInTeleportRange.end(), planet);
        return find != _planetsInTeleportRange.end();
    }

    void PlayerController::dropItem(PlaceableItem *placeableItem)
    {
        _inventory.removeItem(placeableItem);
        _character->insideArea()->addPlaceable(placeableItem, _character->transform().position);
    }

    void PlayerController::checkForTeleportableShips(sf::Vector2f position, const StarSystem &starSystem)
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

    void PlayerController::checkForTeleportablePlanets(sf::Vector2f position, const StarSystem &starSystem)
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

    void PlayerController::checkForGroundInteractables(sf::Vector2f position, const WalkableArea &area)
    {
        // Check existing items
        auto playerPos = _character->transform().position;
        for (auto item : canInteractWith())
        {
            auto dpos = item->transform().position - playerPos;
            auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
            if (distance - interactRangeObjectsSquared() > 0.0f)
            {
                removeCanInteractWith(item);
                if (item->item->isPlayerInRange())
                {
                    item->item->onPlayerLeaves(_session);
                }
            }
        }

        for (auto &iter : area.placedItems())
        {
            if (canInteractWith(iter.get()))
            {
                continue;
            }

            auto dpos = iter->transform().position - playerPos;
            auto distance = dpos.x * dpos.x + dpos.y * dpos.y;
            if (distance - interactRangeObjectsSquared() < 0.0f)
            {
                if (addCanInteractWith(iter.get()))
                {
                    if (!iter->item->isPlayerInRange())
                    {
                        iter->item->onPlayerEnters(_session);
                    }
                }
            }
        }
    }
} // namespace space
