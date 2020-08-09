#include "ui_interactables.hpp"

#include <sstream>

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../game/items/placeable_item.hpp"
#include "../game/items/placed_item.hpp"
#include "../game/walkable_area.hpp"
#include "../game_session.hpp"
#include "../controllers/player_controller.hpp"

namespace space
{
    void UIInteractables::draw(GameSession &session)
    {
        ImGui::Begin("Interactables");

        for (auto canInteractWith : session.playerController().canInteractWith())
        {
            ImGui::Text("%s", canInteractWith->item->definition.name.c_str());
            ImGui::SameLine();

            std::stringstream useSs;
            useSs << "Use##" << canInteractWith->id;
            auto label = useSs.str();

            if (ImGui::Button(label.c_str()))
            {
                canInteractWith->item->execute(session, canInteractWith->transform().position, canInteractWith->area);
            }

            std::stringstream pickupSs;
            pickupSs << "Pickup##" << canInteractWith->id;
            label = pickupSs.str();

            ImGui::SameLine();
            if (ImGui::Button(label.c_str()))
            {
                session.playerController().inventory().addItem(canInteractWith->item);
                canInteractWith->area.removePlaceable(canInteractWith->item->id);
            }
        }
        ImGui::End();
    }
} // namespace space
