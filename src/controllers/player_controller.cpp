#include "player_controller.hpp"

#include <SFML/Window.hpp>

#include "../keyboard.hpp"
#include "../game/ship.hpp"
#include "../game/character.hpp"
#include "../game/planet.hpp"
#include "../game/planet_surface.hpp"

namespace space
{
    PlayerController::PlayerController(GameSession &session) : CharacterController(session)
    {

    }

    void PlayerController::update(sf::Time dt)
    {
        if (_controlling == ControlNone)
        {
            return;
        }

        // _teleportersInRange.clear();

        if (_controlling == ControlShip)
        {
            controlShip(dt);

            if (_ship)
            {
                _teleportersInRange.updateMark();
                checkForInTeleportRange(_ship->transform().position, *_ship->insideArea());
                _teleportersInRange.removeOldMarked();
            }
        }
        else if (_controlling == ControlCharacter)
        {
            if (_character && _character->insideArea())
            {
                updateAnimations(dt);

                _teleportersInRange.updateMark();

                auto insideArea = _character->insideArea();
                checkForInteractables(_character->transform().position, *insideArea);

                auto planetSurface = insideArea->partOfPlanetSurface();
                auto ship = insideArea->partOfShip();
                if (planetSurface != nullptr)
                {
                    auto onPlanet = insideArea->partOfPlanetSurface()->partOfPlanet();
                    checkForInTeleportRange(onPlanet->transform().position, *onPlanet->insideArea());
                }
                else if (ship != nullptr)
                {
                    checkForInTeleportRange(ship->transform().position, *ship->insideArea());
                }

                _teleportersInRange.removeOldMarked();
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

} // namespace space
