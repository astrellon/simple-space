#include "teleporter.hpp"

#include "placed_item.hpp"

#include "../area.hpp"
#include "../planet_surface.hpp"
#include "../ship.hpp"

#include "../../game_session.hpp"
#include "../../engine.hpp"
#include "../../ui/ui_manager.hpp"
#include "../../game-ui/game_ui_manager.hpp"
#include "../../game-ui/game_ui_teleporters_panel.hpp"

namespace space
{
    void Teleporter::execute(CharacterController *controller, GameSession &session, const sf::Vector2f &position, Area &parentArea)
    {
        if (!parentArea.partOfShip() && !parentArea.partOfPlanetSurface())
        {
            std::cout << "Using teleporter outside of ship or planet" << std::endl;
            return;
        }

        if (controller->isPlayer())
        {
            session.engine().gameUIManager().teleportersPanel().teleporters(&controller->teleportersInRange());
        }
    }

    void Teleporter::onPlaced(PlacedItem &placedItem)
    {
        auto type = placedItem.insideArea()->type();
        if (type == AreaType::PlanetSurface)
        {
            _name = placedItem.insideArea()->partOfPlanetSurface()->definition.name;
        }
        else if (type == AreaType::Ship)
        {
            _name = placedItem.insideArea()->partOfShip()->definition.name;
        }
    }

    void Teleporter::onCharacterLeaves(CharacterController *controller, GameSession &session)
    {
        PlaceableItem::onCharacterLeaves(controller, session);

        if (controller->isPlayer())
        {
            session.engine().gameUIManager().teleportersPanel().teleporters(nullptr);
        }
    }
} // namespace space
