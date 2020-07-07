#include "player_controller.hpp"

#include "game/ship.hpp"
#include "game/character.hpp"
#include "keyboard.hpp"

#include "game/items/item.hpp"
#include "game/items/placeable_item.hpp"

namespace space
{
    PlayerController::PlayerController() : _controlling(ControlNone), _inventory()
    {

    }

    void PlayerController::update(sf::Time dt)
    {
        if (_controlling == ControlNone)
        {
            return;
        }
        if (_controlling == ControlShip)
        {
            controlShip(dt);
        }
        else if (_controlling == ControlCharacter)
        {
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

    void PlayerController::addCanInteractWith(PlacedItem *item)
    {
        _canInteractWith.push_back(item);
    }
    void PlayerController::removeCanInteractWith(PlacedItem *item)
    {
        auto find = std::find(_canInteractWith.begin(), _canInteractWith.end(), item);
        if (find != _canInteractWith.end())
        {
            _canInteractWith.erase(find);
        }
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

    void PlayerController::dropItem(PlaceableItem *placeableItem)
    {
        _inventory.removeItem(placeableItem);
        _character->insideArea()->addPlaceable(placeableItem, _character->transform().position);
    }

} // namespace space
