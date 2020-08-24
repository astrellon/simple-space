#include "teleporter.hpp"

#include "../walkable_area.hpp"
#include "../planet_surface.hpp"
#include "../ship.hpp"

#include "../../game_session.hpp"
#include "../../engine.hpp"
#include "../../ui/ui_manager.hpp"
#include "../../ui/ui_teleporter.hpp"

namespace space
{
    void Teleporter::execute(GameSession &session, const sf::Vector2f &position, WalkableArea &parentArea)
    {
        if (!parentArea.partOfShip() && !parentArea.partOfPlanetSurface())
        {
            std::cout << "Using teleporter outside of ship or planet" << std::endl;
            return;
        }

        auto &teleporter = session.engine().uiManager().uiTeleporter();
        teleporter.controller(&session.playerController());
    }

    void Teleporter::onPlaced(PlacedItem &placedItem)
    {
        if (placedItem.area.partOfPlanetSurface())
        {
            _name = placedItem.area.partOfPlanetSurface()->definition.name;
        }
        else if (placedItem.area.partOfShip())
        {
            _name = placedItem.area.partOfShip()->definition.name;
        }
    }

    void Teleporter::onPlayerLeaves(GameSession &session)
    {
        PlaceableItem::onPlayerLeaves(session);

        session.engine().uiManager().uiTeleporter().controller(nullptr);
    }
} // namespace space
