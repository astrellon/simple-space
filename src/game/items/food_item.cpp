#include "food_item.hpp"

#include "../area.hpp"
#include "../../game_session.hpp"
#include "../../engine.hpp"

#include "../../controllers/npc_controller.hpp"

namespace space
{
    void FoodItem::execute(CharacterController &controller, PlacedItem &placed)
    {
        auto npc = dynamic_cast<NpcController *>(&controller);
        if (npc == nullptr)
        {
            return;
        }

        auto dt = controller.session().engine().deltaTime();
        npc->needs().changeHunger(0.2f * dt.asSeconds());
    }
} // space