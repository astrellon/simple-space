#include "chair.hpp"

#include <iostream>

#include "../area.hpp"
#include "../../game_session.hpp"

namespace space
{
    void Chair::execute(CharacterController *controller, GameSession &session, const sf::Vector2f &position, Area &parentArea)
    {
        auto ship = parentArea.partOfShip();
        if (ship == nullptr)
        {
            std::cout << "Using chair outside of ship" << std::endl;
            return;
        }

        if (controller->isPlayer())
        {
            session.setPlayerControllingShip(ship);
        }
        else
        {
            controller->controllingShip(ship);
            controller->controlling(ControlShip);
        }
    }
} // namespace space
