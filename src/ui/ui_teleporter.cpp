#include "ui_teleporter.hpp"

#include <sstream>

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../game/items/teleporter.hpp"
#include "../game/items/placed_item.hpp"
#include "../game/walkable_area.hpp"
#include "../game/ship.hpp"
#include "../game_session.hpp"

namespace space
{
    void UITeleporter::draw(GameSession &session, const std::vector<Ship *> &ships)
    {
        ImGui::Begin("Teleporters");

        for (auto ship : ships)
        {
            ImGui::Text("%s:", ship->definition.name.c_str());

            auto teleporters = ship->findTeleporters();

            for (auto &teleporter : teleporters)
            {
                ImGui::Text("%s", teleporter.item->name().c_str());

                ImGui::SameLine();

                std::stringstream ss;
                ss << "Go!##" << teleporter.item->id;

                auto label = ss.str();

                if (ImGui::Button(label.c_str()))
                {
                    auto &area = teleporter.placed->area;
                    if (area.partOfShip() == nullptr)
                    {
                        std::cout << "Teleport target not on ship!" << std::endl;
                    }
                    else
                    {
                        std::cout << "Teleport to: " << teleporter.item->name() << " @ " << area.partOfShip()->definition.name << std::endl;

                        session.moveCharacter(session.playerController().controllingCharacter(), teleporter.placed->transform().position, &teleporter.placed->area);
                    }
                }
            }
        }

        ImGui::End();
    }
} // namespace space
