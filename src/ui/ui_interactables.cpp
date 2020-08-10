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
            ImGui::Text("%s", canInteractWith->name().c_str());

            for (auto &interaction : canInteractWith->interactions())
            {
                ImGui::SameLine();

                std::stringstream useSs;
                useSs << interaction->label() << "##" << canInteractWith->parentObject()->id;
                auto label = useSs.str();

                if (ImGui::Button(label.c_str()))
                {
                    interaction->execute(session);
                }
            }
        }
        ImGui::End();
    }
} // namespace space
