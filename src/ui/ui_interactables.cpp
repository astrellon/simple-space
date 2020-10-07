#include "ui_interactables.hpp"

#include <sstream>

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../game/items/placeable_item.hpp"
#include "../game/items/placed_item.hpp"
#include "../game/area.hpp"
#include "../game_session.hpp"
#include "../engine.hpp"
#include "../controllers/player_controller.hpp"

namespace space
{
    UIInteractables::UIInteractables() : UIPanel("Interactables")
    {
        size = ImVec2(200, 160);
        position.x = 20;
        show = true;
    }

    bool UIInteractables::isOpen(Engine &engine)
    {
        if (!engine.currentSession())
        {
            return false;
        }

        auto &player = engine.currentSession()->playerController();
        return show && player.controlling() != ControlShip && !engine.currentSession()->dialogueManager().isInDialogue();
    }

    void UIInteractables::checkPosition(Engine &engine)
    {
        auto renderSize = engine.renderSize();
        position.y = renderSize.y - size.y - 20;
    }

    void UIInteractables::doDraw(Engine &engine)
    {
        ImGui::Text("Interactables");

        if (!engine.currentSession())
        {
            return;
        }

        auto &session = *engine.currentSession();
        auto &player = session.playerController();
        for (auto canInteractWith : player.canInteractWithInRange())
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
    }
} // namespace space
