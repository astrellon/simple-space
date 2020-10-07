#include "use_item_action.hpp"

#include "../items/placeable_item.hpp"
#include "../items/placed_item.hpp"
#include "../../space_transform.hpp"

namespace space
{
    UseItemAction::UseItemAction(PlacedItem *item) : _placedItem(item)
    {

    }

    void UseItemAction::execute(GameSession &session)
    {
        _placedItem->item->execute(session, _placedItem->transform().position, *_placedItem->insideArea());
    }
} // namespace space
