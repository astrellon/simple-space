#include "ui_object_edit.hpp"

#include "../debug/draw_debug.hpp"

#include "../imgui/imgui.h"
#include "../game/space_object.hpp"
#include "../game/star_system.hpp"
#include "../game/ship.hpp"
#include "../game/planet_surface.hpp"
#include "../definitions/compendium_definition.hpp"

namespace space
{
    UIObjectEdit::UIObjectEdit() : spaceObject(nullptr), _selectHandler(nullptr)
    {

    }

    void UIObjectEdit::draw()
    {
        if (!spaceObject)
        {
            ImGui::Text("No selected object");
            return;
        }

        ImGui::Text("Id: %s", spaceObject->id.c_str());

        auto typeStr = toString(spaceObject->type());
        ImGui::Text("Type: %s", typeStr.c_str());
        if (spaceObject->compendiumDefinition())
        {
            ImGui::Text("CompendiumId: %s", spaceObject->compendiumDefinition()->id.c_str());
        }
        else
        {
            ImGui::Text("CompendiumId: None");
        }

        auto insideArea = spaceObject->insideArea();
        if (insideArea)
        {
            auto type = insideArea->type();
            ImGui::Text("Inside Area: %s", areaTypeString(type));
            ImGui::Text("- %s", insideArea->partOfObject()->id.c_str());
            ImGui::SameLine();
            if (ImGui::Button("Select##InsideArea"))
            {
                if (_selectHandler)
                {
                    _selectHandler(insideArea->partOfObject());
                }
            }
        }
        else
        {
            ImGui::Text("Inside Area: Not inside an area");
        }

        auto &trans = spaceObject->transform();
        inputVector2f("Position: ", trans.position);
        ImGui::InputFloat("Rotation deg", &trans.rotation);

        if (DrawDebug::focusOnObject == spaceObject)
        {
            if (ImGui::Button("Unfocus"))
            {
                DrawDebug::focusOnObject = nullptr;
            }
        }
        else if (ImGui::Button("Focus on this object"))
        {
            DrawDebug::focusOnObject = spaceObject;
        }

        IHasArea *hasArea = dynamic_cast<IHasArea *>(spaceObject);
        if (hasArea)
        {
            drawArea(hasArea->area());
        }
    }

    void UIObjectEdit::drawArea(Area &area)
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

    void UIObjectEdit::drawLayer(DrawLayer &layer)
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
                if (_selectHandler)
                {
                    _selectHandler(obj);
                }
            }
        }
    }

    const char *UIObjectEdit::areaTypeString(AreaType type)
    {
        switch (type)
        {
            case AreaType::StarSystem: return "Star System";
            case AreaType::Ship: return "Ship";
            case AreaType::PlanetSurface: return "Planet Surface";
            default: return "Unknown";
        }
    }

    bool UIObjectEdit::inputVector2f(const std::string &label, sf::Vector2f &input)
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