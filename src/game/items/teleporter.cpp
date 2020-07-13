#include "teleporter.hpp"

#include "../walkable_area.hpp"
#include "../../game_session.hpp"

namespace space
{
    void Teleporter::execute(GameSession &session, const sf::Vector2f &position, WalkableArea &parentArea)
    {
        if (!parentArea.partOfShip() && !parentArea.partOfPlanetSurface())
        {
            std::cout << "Using teleporter outside of ship or planet" << std::endl;
            return;
        }

        session.showTeleporters = true;
    }

    void Teleporter::onPlayerLeaves(GameSession &session)
    {
        PlaceableItem::onPlayerLeaves(session);

        session.showTeleporters = false;
    }
} // namespace space
