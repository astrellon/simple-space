#include "chair.hpp"

#include <iostream>

#include "../area.hpp"
#include "../../game_session.hpp"

namespace space
{
    const ItemType Chair::TypeValue = ItemType::Chair;

    void Chair::execute(CharacterController &controller, PlacedItem &placed)
    {
        auto ship = placed.insideArea()->partOfShip();
        if (ship == nullptr)
        {
            std::cout << "Using chair outside of ship" << std::endl;
            return;
        }

        if (controller.isPlayer())
        {
            controller.session().setPlayerControllingShip(ship);
        }
        else
        {
            controller.controllingShip(ship);
            controller.controlling(ControlShip);
        }
    }
} // namespace space
