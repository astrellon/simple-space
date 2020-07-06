#include "ui_inventory.hpp"

#include <sstream>

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../game/items/placeable_item.hpp"
#include "../game/items/placed_item.hpp"
#include "../game/inventory.hpp"
#include "../player_controller.hpp"

namespace space
{
    void UIInventory::draw(PlayerController &player)
    {
        ImGui::Begin("Inventory");
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

        ImGui::End();
    }
} // namespace space
