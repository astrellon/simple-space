#include "teleporter.hpp"

#include "../walkable_area.hpp"
#include "../../game_session.hpp"

namespace space
{
    void Teleporter::execute(GameSession &session, const sf::Vector2f &position, WalkableArea &parentArea)
    {
        auto ship = parentArea.partOfShip();
        if (ship == nullptr)
        {
            std::cout << "Using teleporter outside of ship" << std::endl;
            return;
        }
    }
} // namespace space
