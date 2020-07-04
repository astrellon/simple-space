#include "chair.hpp"

#include <iostream>

#include "../walkable_area.hpp"
#include "../../game_session.hpp"

namespace space
{
    void Chair::execute(GameSession &session, const sf::Vector2f &position, WalkableArea &parentArea)
    {
        auto ship = parentArea.partOfShip();
        if (ship == nullptr)
        {
            std::cout << "Using chair outside of ship" << std::endl;
            return;
        }

        session.setPlayerControllingShip(ship);
    }
} // namespace space
