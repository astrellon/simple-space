#include "ui_interactables.hpp"

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../game/items/placeable_item.hpp"
#include "../game/items/placed_item.hpp"
#include "../game/walkable_area.hpp"
#include "../game_session.hpp"
#include "../player_controller.hpp"

namespace space
{
    void UIInteractables::draw(GameSession &session)
    {
        ImGui::Begin("Interactables");

        for (auto canInteractWith : session.playerController().canInteractWith())
        {
            ImGui::Text("%s", canInteractWith->item->definition.name.c_str());
            ImGui::SameLine();
            if (ImGui::Button("Use"))
            {
                canInteractWith->item->execute(session, canInteractWith->transform().position, canInteractWith->area);
            }

            ImGui::SameLine();
            if (ImGui::Button("Pickup"))
            {
                session.playerController().inventory().addItem(canInteractWith->item);
                canInteractWith->area.removePlaceable(canInteractWith->item->id);
            }
        }
        ImGui::End();
    }
} // namespace space
