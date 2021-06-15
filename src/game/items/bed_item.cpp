#include "bed_item.hpp"

#include "../area.hpp"
#include "../../game_session.hpp"
#include "../../engine.hpp"

#include "../../controllers/npc_controller.hpp"

namespace space
{
    void BedItem::execute(CharacterController &controller, PlacedItem &placed)
    {
        auto npc = dynamic_cast<NpcController *>(&controller);
        if (npc == nullptr)
        {
            return;
        }

        auto dt = controller.session().engine().deltaTime();
        npc->needs().changeEnergy(0.01f * dt.asSeconds());
    }
} // space