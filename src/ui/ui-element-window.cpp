#include "ui-element-window.hpp"

#include <sstream>

#include <yoga/Yoga.h>

#include "../engine.hpp"
#include "../game-ui/game-ui-manager.hpp"
#include "../game-ui/ui-element.hpp"
#include "../utils.hpp"

namespace space
{
    UIElementWindow::UIElementWindow() : UIWindow("UIElement"), _selectedElement(nullptr)
    {

    }

    void UIElementWindow::doDraw(Engine &engine)
    {
        auto width = ImGui::GetWindowContentRegionWidth();
        auto middle = 0.3f;
        auto height = ImGui::GetWindowContentRegionMax().y - 30;

        //ImGui::BeginChild("ElementsList", ImVec2(width * middle, height), false);
        // auto &objects = engine.gameUIManager().rootElement();

        // auto node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        // for (auto &object : objects)
        // {
        //     ImGui::TreeNodeEx((void *)object.get(), node_flags, "%s", object.get()->id.c_str());
        //     if (ImGui::IsItemClicked())
        //     {
        //         _selectedObjectId = object.get()->id;
        //     }
        // }
    }
}