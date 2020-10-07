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
        auto session = engine.currentSession();
        auto character = _characterController->controllingCharacter();
        auto &teleporters = _characterController->teleportersInRange();

        for (auto &teleporter : teleporters)
        {
            ImGui::Text("%s", teleporter.item->name().c_str());
        }
        // auto ships = _characterController->shipsInTeleportRange();
        // auto planets = _characterController->planetsInTeleportRange();

        // ImGui::Text("Ships:");

        // for (auto ship : ships)
        // {
        //     ImGui::Text("%s:", ship->definition.name.c_str());

        //     auto teleporters = ship->area().findTeleporters();

        //     for (auto &teleporter : teleporters)
        //     {
        //         std::stringstream ss;
        //         ss << teleporter.item->name() << "##" << teleporter.item->id;

        //         auto label = ss.str();

        //         if (ImGui::Button(label.c_str()))
        //         {
        //             auto &area = teleporter.placed->area;
        //             if (area.partOfShip() == nullptr)
        //             {
        //                 std::cout << "Teleport target not on ship!" << std::endl;
        //             }
        //             else
        //             {
        //                 std::cout << "Teleport to: " << teleporter.item->name() << " @ " << area.partOfShip()->definition.name << std::endl;

        //                 session->moveCharacter(_characterController->controllingCharacter(), teleporter.placed->transform().position, &teleporter.placed->area);
        //             }
        //         }
        //     }
        // }

        // ImGui::Text("Planets:");

        // for (auto planet : planets)
        // {
        //     if (planet->planetSurfaces().size() == 0)
        //     {
        //         continue;
        //     }

        //     ImGui::Text("%s: %d", planet->definition.name.c_str(), (int)planet->planetSurfaces().size());

        //     for (auto surface : planet->planetSurfaces())
        //     {
        //         auto teleporters = surface->area().findTeleporters();

        //         for (auto &teleporter : teleporters)
        //         {
        //             std::stringstream ss;
        //             ss << teleporter.item->name() << "##" << teleporter.item->id;

        //             auto label = ss.str();

        //             if (ImGui::Button(label.c_str()))
        //             {
        //                 auto &area = teleporter.placed->area;
        //                 if (area.partOfPlanetSurface() == nullptr)
        //                 {
        //                     std::cout << "Teleport target not on planet!" << std::endl;
        //                 }
        //                 else
        //                 {
        //                     std::cout << "Teleport to: " << teleporter.item->name() << " @ " << area.partOfPlanetSurface()->definition.name << std::endl;

        //                     session->moveCharacter(_characterController->controllingCharacter(), teleporter.placed->transform().position, &teleporter.placed->area, true);
        //                 }
        //             }
        //         }
        //     }
        // }
    }
} // namespace space
