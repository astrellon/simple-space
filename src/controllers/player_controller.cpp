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

        // if (_character && _character->insideArea())
        // {
        //     auto area = _character->insideArea();
        //     auto planetSurface = area->partOfPlanetSurface();
        //     if (planetSurface != nullptr && planetSurface->partOfPlanet() != nullptr)
        //     {
        //         auto onPlanet = area->partOfPlanetSurface()->partOfPlanet();
        //         // checkForTeleportableShips(onPlanet->transform().position, *onPlanet->starSystem());
        //         // checkForTeleportablePlanets(onPlanet->transform().position, *onPlanet->starSystem());
        //     }
        //     else if (area->partOfShip() != nullptr)
        //     {
        //         auto insideShip = area->partOfShip();
        //         checkForTeleportableShips(insideShip->transform().position, *insideShip->starSystem());
        //         checkForTeleportablePlanets(insideShip->transform().position, *insideShip->starSystem());
        //     }
        // }
        // else
        // {
        //     clearPlanetsInTeleportRange();
        //     clearShipsInTeleportRange();
        // }

        if (_controlling == ControlShip)
        {
            controlShip(dt);
        }
        else if (_controlling == ControlCharacter)
        {
            if (_character && _character->insideArea())
            {
                updateAnimations(dt);
                // checkForGroundInteractables(_character->transform().position, *_character->insideArea());
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
