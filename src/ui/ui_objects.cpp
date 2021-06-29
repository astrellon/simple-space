#include "ui_objects.hpp"

#include <sstream>

#include "../engine.hpp"
#include "../game_session.hpp"
#include "../game/space_object.hpp"
#include "../game/star_system.hpp"
#include "../game/planet_surface.hpp"
#include "../game/ship.hpp"
#include "../utils.hpp"
#include "../definitions/compendium_definition.hpp"
#include "../debug/draw_debug.hpp"

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

        auto typeStr = toString(spaceObject.type());
        ImGui::Text("Type: %s", typeStr.c_str());
        if (spaceObject.compendiumDefinition())
        {
            ImGui::Text("CompendiumId: %s", spaceObject.compendiumDefinition()->id.c_str());
        }
        else
        {
            ImGui::Text("CompendiumId: None");
        }

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
        inputVector2f("Position: ", trans.position);
        ImGui::InputFloat("Rotation deg", &trans.rotation);

        if (DrawDebug::focusOnObject == &spaceObject)
        {
            if (ImGui::Button("Unfocus"))
            {
                DrawDebug::focusOnObject = nullptr;
            }
        }
        else if (ImGui::Button("Focus on this object"))
        {
            DrawDebug::focusOnObject = &spaceObject;
        }

        StarSystem *starSystem;
        PlanetSurface *planetSurface;
        Ship *ship;

        if (spaceObject.tryCast(starSystem))
        {
            drawArea(starSystem->area());
        }
        else if (spaceObject.tryCast(planetSurface))
        {
            drawArea(planetSurface->area());
        }
        else if (spaceObject.tryCast(ship))
        {
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
            auto typeStr = toString(obj->type());
            ImGui::Text("  - %s: %s", obj->id.c_str(), typeStr.c_str());
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

    bool UIObjects::inputVector2f(const std::string &label, sf::Vector2f &input)
    {
        float v[2] = {input.x, input.y};
        if (ImGui::InputFloat2(label.c_str(), v))
        {
            input.x = v[0];
            input.y = v[1];
            return true;
        }
        return false;
    }
} // space