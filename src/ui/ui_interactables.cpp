#include "ui_interactables.hpp"

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../game/items/placeable_item.hpp"
#include "../game/items/placed_item.hpp"
#include "../game_session.hpp"
#include "../player_controller.hpp"

namespace space
{
    void UIInteractables::drawInteractables(GameSession &session)
    {
        ImGui::Begin("Interactables");

        for (auto &canInteractWith : session.playerController().canInteractWith())
        {
            if (ImGui::Button(canInteractWith->item->definition.name.c_str()))
            {
                canInteractWith->item->execute(session, canInteractWith->transform().position, canInteractWith->area);
            }
        }
        ImGui::End();
    }
} // namespace space
