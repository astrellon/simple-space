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
    UIObjects::UIObjects() : UIWindow("Objects"), _selectedObjectEditor(std::make_unique<UIObjectEdit>())
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
                selectedObjectId = object.get()->id;
            }
        }

        ImGui::EndChild();

        if (selectedObjectId.size() > 0)
        {
            ImGui::SameLine();
            ImGui::BeginChild("SelectedObject", ImVec2(width * (1 - middle), height), false);

            SpaceObject *selectedObject;
            if (!engine.currentSession()->tryGetSpaceObject(selectedObjectId, &selectedObject))
            {
                ImGui::Text("Selected object doesn't exist anymore");
                _selectedObjectEditor->spaceObject = nullptr;
            }
            else
            {
                _selectedObjectEditor->spaceObject = selectedObject;
            }

            _selectedObjectEditor->draw();

            ImGui::EndChild();
        }
    }
} // space