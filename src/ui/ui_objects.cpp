#include "ui_objects.hpp"

#include <sstream>

#include "../engine.hpp"
#include "../game_session.hpp"
#include "../game/star_system.hpp"
#include "../game/planet_surface.hpp"
#include "../game/ship.hpp"
#include "../utils.hpp"

namespace space
{
    UIObjects::UIObjects() : UIWindow("Objects")
    {

    }

    void UIObjects::doDraw(Engine &engine)
    {
        auto width = ImGui::GetWindowContentRegionWidth();
        auto middle = 0.3f;
        auto height = ImGui::GetWindowContentRegionMax().y - 30;

        ImGui::BeginChild("ObjectsList", ImVec2(width * middle, height), false);
        auto &objects = engine.currentSession()->spaceObjects();

        auto node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        for (auto &object : objects)
        {
            ImGui::TreeNodeEx((void *)object.get(), node_flags, "%s", object.get()->id.c_str());
            if (ImGui::IsItemClicked())
            {
                _selectedObjectId = object.get()->id;
            }
        }

        ImGui::EndChild();

        if (_selectedObjectId.size() > 0)
        {
            ImGui::SameLine();
            ImGui::BeginChild("SelectedObject", ImVec2(width * (1 - middle), height), false);

            SpaceObject *selectedObject;
            if (!engine.currentSession()->tryGetSpaceObject(_selectedObjectId, &selectedObject))
            {
                ImGui::Text("Selected object doesn't exist anymore");
            }
            else
            {
                drawSpaceObject(*selectedObject);
            }

            ImGui::EndChild();
        }
    }

    void UIObjects::drawSpaceObject(SpaceObject &spaceObject)
    {
        ImGui::Text("Id: %s", spaceObject.id.c_str());
        ImGui::Text("Type: %s", spaceObject.type().c_str());

        auto insideArea = spaceObject.insideArea();
        if (insideArea)
        {
            auto type = insideArea->type();
            ImGui::Text("Inside Area: %s", areaTypeString(type));
            ImGui::Text("- %s", insideArea->partOfObject()->id.c_str());
            ImGui::SameLine();
            if (ImGui::Button("Select##InsideArea"))
            {
                _selectedObjectId = insideArea->partOfObject()->id;
            }
        }
        else
        {
            ImGui::Text("Inside Area: Not inside an area");
        }

        auto &trans = spaceObject.transform();
        ImGui::Text("Position: %f, %f", trans.position.x, trans.position.y);
        ImGui::Text("Rotation: %f deg", trans.rotation);

        if (spaceObject.type() == StarSystem::SpaceObjectType())
        {
            auto starSystem = dynamic_cast<StarSystem *>(&spaceObject);
            drawArea(starSystem->area());
        }
        else if (spaceObject.type() == PlanetSurface::SpaceObjectType())
        {
            auto planetSurface = dynamic_cast<PlanetSurface *>(&spaceObject);
            drawArea(planetSurface->area());
        }
        else if (spaceObject.type() == Ship::SpaceObjectType())
        {
            auto ship = dynamic_cast<Ship *>(&spaceObject);
            drawArea(ship->area());
        }
    }

    void UIObjects::drawArea(Area &area)
    {
        ImGui::Text("Area");
        ImGui::Text("- Background:");
        drawLayer(area.background());
        ImGui::NewLine();

        ImGui::Text("- Main:");
        drawLayer(area.main());
        ImGui::NewLine();

        ImGui::Text("- Foreground:");
        drawLayer(area.foreground());
        ImGui::NewLine();
    }

    void UIObjects::drawLayer(DrawLayer &layer)
    {
        for (auto obj : layer.drawables())
        {
            ImGui::Text("  - %s: %s", obj->id.c_str(), obj->type().c_str());
            ImGui::SameLine();

            std::stringstream ss;
            ss << "Select##" << obj->id;
            if (ImGui::Button(ss.str().c_str()))
            {
                _selectedObjectId = obj->id;
            }
        }
    }

    const char *UIObjects::areaTypeString(AreaType type)
    {
        switch (type)
        {
            case AreaType::StarSystem: return "Star System";
            case AreaType::Ship: return "Ship";
            case AreaType::PlanetSurface: return "Planet Surface";
            default: return "Unknown";
        }
    }
} // space