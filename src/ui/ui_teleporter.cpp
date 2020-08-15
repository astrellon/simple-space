#include "ui_teleporter.hpp"

#include <sstream>

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../game/items/teleporter.hpp"
#include "../game/items/placed_item.hpp"
#include "../game/walkable_area.hpp"
#include "../game/ship.hpp"
#include "../game/planet.hpp"
#include "../game/planet_surface.hpp"

#include "../game_session.hpp"
#include "../engine.hpp"

#include "ui_manager.hpp"

namespace space
{
    UITeleporter::UITeleporter() : UIWindow("Teleporters"), _characterController(nullptr)
    {
        size = ImVec2(160, 160);
        position = ImVec2(20, 20);
    }

    bool UITeleporter::isOpen(Engine &engine)
    {
        if (!engine.currentSession())
        {
            return false;
        }

        return !engine.currentSession()->dialogueManager().isInDialogue() && _characterController != nullptr;
    }

    void UITeleporter::checkPosition(Engine &engine)
    {
        auto renderSize = engine.renderSize();
        position.y = renderSize.y - size.y - 190;
    }

    void UITeleporter::doDraw(Engine &engine)
    {
        auto session = engine.currentSession();
        auto ships = _characterController->shipsInTeleportRange();
        auto planets = _characterController->planetsInTeleportRange();

        ImGui::Text("Ships:");

        for (auto ship : ships)
        {
            ImGui::Text("%s:", ship->definition.name.c_str());

            auto teleporters = ship->walkableArea().findTeleporters();

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

                        session->moveCharacter(_characterController->controllingCharacter(), teleporter.placed->transform().position, &teleporter.placed->area);
                    }
                }
            }
        }

        ImGui::Text("Planets:");

        for (auto planet : planets)
        {
            ImGui::Text("%s: %d", planet->definition.name.c_str(), (int)planet->planetSurfaces().size());

            for (auto surface : planet->planetSurfaces())
            {
                auto teleporters = surface->walkableArea().findTeleporters();

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
                        if (area.partOfPlanetSurface() == nullptr)
                        {
                            std::cout << "Teleport target not on planet!" << std::endl;
                        }
                        else
                        {
                            std::cout << "Teleport to: " << teleporter.item->name() << " @ " << area.partOfPlanetSurface()->definition.name << std::endl;

                            session->moveCharacter(_characterController->controllingCharacter(), teleporter.placed->transform().position, &teleporter.placed->area);
                        }
                    }
                }
            }
        }
    }
} // namespace space
