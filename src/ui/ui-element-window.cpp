#include "ui-element-window.hpp"

#include <sstream>

#include <yoga/Yoga.h>

#include "../engine.hpp"
#include "../game-ui/game-ui-manager.hpp"
#include "../game-ui/ui-element.hpp"
#include "../utils.hpp"
#include "../debug/draw_debug.hpp"

namespace space
{
    const char *elementTypeName(ElementType input)
    {
        switch (input)
        {
            default:
            case ElementType::Unknown: return "Unknown";
            case ElementType::General: return "General";
            case ElementType::Image: return "Image";
            case ElementType::Root: return "Root";
            case ElementType::Text: return "Text";
            case ElementType::NineSliceImage: return "NineSliceImage";
        }
    }

    bool ImGuiYGValue(const char *label, YGValue &input, bool includeAuto)
    {
        ImGui::Text("%s", label);
        ImGui::SameLine();

        ImGui::PushID(label);

        auto unit = (int)input.unit;
        auto changed = false;

        changed |= ImGui::RadioButton("Unset", &unit, YGUnitUndefined);
        ImGui::SameLine();

        if (includeAuto)
        {
            changed |= ImGui::RadioButton("Auto", &unit, YGUnitAuto);
            ImGui::SameLine();
        }

        changed |= ImGui::RadioButton("Percent", &unit, YGUnitPercent);
        ImGui::SameLine();
        changed |= ImGui::RadioButton("Point", &unit, YGUnitPoint);

        input.unit = (YGUnit)unit;

        if (unit == YGUnitPercent)
        {
            ImGui::SameLine();
            changed |= ImGui::SliderFloat("", &input.value, 0, 100);
        }
        else if (unit == YGUnitPoint)
        {
            ImGui::SameLine();
            changed |= ImGui::InputFloat("", &input.value, 1, 1);
        }

        if (changed && input.unit != YGUnitUndefined && isnan(input.value))
        {
            input.value = 0.0f;
        }

        ImGui::PopID();

        return changed;
    }

    void ImGuiYGPadding(const char *label, YGEdge edge, UIElement &element)
    {
        auto padding = element.padding(edge);
        if (ImGuiYGValue(label, padding, false))
        {
            element.padding(edge, padding);
        }
    }

    void ImGuiYGPaddingAll(UIElement &element)
    {
        ImGuiYGPadding("Padding Top", YGEdgeTop, element);
        ImGuiYGPadding("Padding Right", YGEdgeRight, element);
        ImGuiYGPadding("Padding Bottom", YGEdgeBottom, element);
        ImGuiYGPadding("Padding Left", YGEdgeLeft, element);
    }

    void ImGuiYGMargin(const char *label, YGEdge edge, UIElement &element)
    {
        auto margin = element.margin(edge);
        if (ImGuiYGValue(label, margin, true))
        {
            element.margin(edge, margin);
        }
    }

    void ImGuiYGMarginAll(UIElement &element)
    {
        ImGuiYGMargin("Margin Top", YGEdgeTop, element);
        ImGuiYGMargin("Margin Right", YGEdgeRight, element);
        ImGuiYGMargin("Margin Bottom", YGEdgeBottom, element);
        ImGuiYGMargin("Margin Left", YGEdgeLeft, element);
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
                DrawDebug::highlightElement = element.get();
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

        ImGuiYGPaddingAll(element);
        ImGuiYGMarginAll(element);
        //YGValue padding[4] = {element.padding(YGEdgeTop), element.padding(YGEdgeRight), element.padding(YGEdgeBottom), element.padding(YGEdgeLeft)};
        // if (ImGui::InputFloat4("Padding TRBL:", padding))
        // {
        //     element.padding(padding[0], padding[1], padding[2], padding[3]);
        // }
    }
}