#include "pickup_action.hpp"

#include "../../game_session.hpp"
#include "../area.hpp"
#include "../items/placed_item.hpp"
#include "../items/placeable_item.hpp"

namespace space
{
    PickupAction::PickupAction(PlacedItem *item) : _placedItem(item)
    {

    }

    void PickupAction::execute(GameSession &session)
    {
        session.playerController().inventory().addItem(_placedItem->item);
        _placedItem->insideArea()->removeObject(_placedItem);
    }
} // namespace space
