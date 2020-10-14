#include "ui_teleporter.hpp"

#include <sstream>

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../game/items/teleporter.hpp"
#include "../game/items/placed_item.hpp"
#include "../game/area.hpp"
#include "../game/character.hpp"
#include "../game/ship.hpp"
#include "../game/planet.hpp"
#include "../game/planet_surface.hpp"

#include "../game_session.hpp"
#include "../engine.hpp"

#include "ui_manager.hpp"

namespace space
{
    UITeleporter::UITeleporter() : UIPanel("Teleporters"), _characterController(nullptr)
    {
        size = ImVec2(200, 160);
        position = ImVec2(20, 20);
        show = true;
    }

    bool UITeleporter::isOpen(Engine &engine)
    {
        if (!engine.currentSession())
        {
            return false;
        }

        return show && !engine.currentSession()->dialogueManager().isInDialogue() && _characterController != nullptr && _characterController->controlling() == ControlCharacter;
    }

    void UITeleporter::checkPosition(Engine &engine)
    {
        auto renderSize = engine.renderSize();
        position.y = renderSize.y - size.y - 190;
    }

    void UITeleporter::doDraw(Engine &engine)
    {
        ImGui::Text("Teleporters");

        auto session = engine.currentSession();
        auto character = _characterController->controllingCharacter();
        auto &teleporters = _characterController->teleportersInRange();

        for (auto &teleporter : teleporters)
        {
            std::stringstream ss;
            ss << teleporter.item->name() << "##" << teleporter.item->id;

            auto label = ss.str();

            if (ImGui::Button(label.c_str()))
            {
                auto placed = teleporter.placed;
                session->moveSpaceObject(character, placed->transform().position, placed->insideArea(), true);
            }
        }
    }
} // namespace space
