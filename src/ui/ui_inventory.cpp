#include "ui_inventory.hpp"

#include <sstream>

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../game/items/placeable_item.hpp"
#include "../game/items/placed_item.hpp"
#include "../game/inventory.hpp"
#include "../controllers/player_controller.hpp"
#include "../engine.hpp"
#include "../game_session.hpp"

namespace space
{
    UIInventory::UIInventory() : UIPanel("Inventory")
    {
        size = ImVec2(200, 160);
        position = ImVec2(20, 20);
        show = true;
    }

    bool UIInventory::isOpen(Engine &engine)
    {
        if (!engine.currentSession())
        {
            return false;
        }

        auto &player = engine.currentSession()->playerController();
        return show && player.controlling() != ControlShip && !engine.currentSession()->dialogueManager().isInDialogue();
    }

    void UIInventory::doDraw(Engine &engine)
    {
        ImGui::Text("Inventory");

        if (!engine.currentSession())
        {
            return;
        }

        auto &player = engine.currentSession()->playerController();
        for (auto &item : player.inventory().items())
        {
            ImGui::Text("%s", item->definition.name.c_str());

            auto placeableItem = dynamic_cast<PlaceableItem *>(item);
            if (placeableItem != nullptr)
            {
                ImGui::SameLine();

                std::stringstream ss;
                ss << "Drop##" << item->id;

                auto label = ss.str();

                if (ImGui::Button(label.c_str()))
                {
                    player.dropItem(placeableItem);
                }
            }
        }
    }
} // namespace space
