#include "ui-element-window.hpp"

#include <sstream>

#include <yoga/Yoga.h>

#include "../engine.hpp"
#include "../game-ui/game-ui-manager.hpp"
#include "../game-ui/ui-element.hpp"
#include "../utils.hpp"

namespace space
{
    const char *elementTypeName(ElementType input)
    {
        switch (input)
        {
            default:
            case ElementType::Unknown: return "Unknown";
            case ElementType::Image: return "Image";
            case ElementType::Root: return "Root";
            case ElementType::Text: return "Text";
        }
    }

    UIElementWindow::UIElementWindow() : UIWindow("UIElement"), _selectedElement(nullptr)
    {

    }

    void UIElementWindow::doDraw(Engine &engine)
    {
        auto width = ImGui::GetWindowContentRegionWidth();
        auto middle = 0.3f;
        auto height = ImGui::GetWindowContentRegionMax().y - 30;

        ImGui::BeginChild("ElementsList", ImVec2(width * middle, height), false);
        auto &allElements = engine.gameUIManager().allElements();

        auto node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        for (auto &element : allElements)
        {
            auto elementTypeStr = elementTypeName(element->elementType());
            ImGui::TreeNodeEx((void *)element.get(), node_flags, "%s", elementTypeStr);
            if (ImGui::IsItemClicked())
            {
                _selectedElement = element.get();
            }
        }

        ImGui::EndChild();

        if (_selectedElement != nullptr)
        {
            ImGui::SameLine();
            ImGui::BeginChild("SelectedElement", ImVec2(width * (1 - middle), height), false);

            drawElement(*_selectedElement);

            ImGui::EndChild();
        }
    }

    void UIElementWindow::drawElement(UIElement &element)
    {
        ImGui::Text("Type: %s", elementTypeName(element.elementType()));

        ImGui::Text("Position TRBL: %f, %f, %f, %f", element.layoutTop(), element.layoutRight(), element.layoutBottom(), element.layoutLeft());
    }
}