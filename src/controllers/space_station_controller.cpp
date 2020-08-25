#include "space_station_controller.hpp"

#include "../game/ship.hpp"

namespace space
{
    SpaceStationController::SpaceStationController(GameSession &session) : CharacterController(session)
    {
        _controlling = ControlShip;
    }

    void SpaceStationController::update(sf::Time dt)
    {
        if (_controlling == ControlShip && _ship)
        {
            _ship->rotateInput = 1.0f;
        }
    }
}