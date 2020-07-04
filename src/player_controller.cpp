#include "player_controller.hpp"

#include "game/ship.hpp"
#include "game/character.hpp"
#include "keyboard.hpp"

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

        _character->moveInput = moveInput;
        _character->rotateInput = rotateInput;
    }
} // namespace space
