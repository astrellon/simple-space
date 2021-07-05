#include "ui-element-window.hpp"

#include <sstream>

#include <yoga/Yoga.h>

#include "../engine.hpp"
#include "../game-ui/game_ui_manager.hpp"
#include "../game-ui/ui_element.hpp"
#include "../game-ui/ui_text_element.hpp"
#include "../utils.hpp"
#include "../debug/draw_debug.hpp"



namespace space
{
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

        for (auto &element : allElements)
        {
            auto nodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            if (element.get() == _selectedElement)
            {
                nodeFlags |= ImGuiTreeNodeFlags_Selected;
            }

            auto elementTypeStr = element->elementType();
            ImGui::TreeNodeEx((void *)element.get(), nodeFlags, "%s", elementTypeStr);
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
        ImGui::Text("Type: %s", element.elementType());
        ImGui::Text("Width x Height: %f, %f", element.layoutWidth(), element.layoutHeight());
        ImGui::Text("Position TRBL: %f, %f, %f, %f", element.layoutTop(), element.layoutRight(), element.layoutBottom(), element.layoutLeft());
        ImGui::Text("Border TRBL: %f, %f, %f, %f", element.layoutBorder(YGEdgeTop), element.layoutBorder(YGEdgeRight), element.layoutBorder(YGEdgeBottom), element.layoutBorder(YGEdgeLeft));
        // ImGui::Text("Margin TRBL: %f, %f, %f, %f", element.margin(YGEdgeTop), element.layoutBorder(YGEdgeRight), element.layoutBorder(YGEdgeBottom), element.layoutBorder(YGEdgeLeft));

        auto flexBasis = element.flexBasis();
        if (ImGuiYGValue("Flex Basis: ", flexBasis, true))
        {
            element.flexBasis(flexBasis);
        }

        auto flexGrow = element.flexGrow();
        if (ImGui::SliderFloat("Flex Grow: ", &flexGrow, 0.0f, 3.0f))
        {
            element.flexGrow(flexGrow);
        }

        auto flexShrink = element.flexShrink();
        if (ImGui::SliderFloat("Flex Shrink: ", &flexShrink, 0.0f, 3.0f))
        {
            element.flexShrink(flexShrink);
        }

        auto justifyContent = element.justifyContent();
        static const char *justifyContentLabels = "Flex Start\0Flex End\0Center\0Space Around\0Space Between\0Space Evenly\0\0";
        static std::vector<YGJustify> justifyContentValues = { YGJustifyFlexStart, YGJustifyFlexEnd, YGJustifyCenter, YGJustifySpaceAround, YGJustifySpaceBetween, YGJustifySpaceEvenly };
        auto justifyContentIndex = (int)Utils::findIndex(justifyContentValues, justifyContent);
        if (justifyContentIndex >= 0)
        {
            if (ImGui::Combo("Justify Content", &justifyContentIndex, justifyContentLabels))
            {
                element.justifyContent(justifyContentValues[justifyContentIndex]);
            }
        }

        auto alignItems = element.alignItems();
        static const char *alignLabels = "Auto\0Baseline\0Center\0Flex Start\0Flex End\0Space Around\0Space Between\0Stretch\0\0";
        static std::vector<YGAlign> alignValues = { YGAlignAuto, YGAlignBaseline, YGAlignCenter, YGAlignFlexStart, YGAlignFlexEnd, YGAlignSpaceAround, YGAlignSpaceBetween, YGAlignStretch };

        auto alignItemsIndex = (int)Utils::findIndex(alignValues, alignItems);
        if (alignItemsIndex >= 0)
        {
            if (ImGui::Combo("Align Items", &alignItemsIndex, alignLabels))
            {
                element.alignItems(alignValues[alignItemsIndex]);
            }
        }

        auto alignContent = element.alignContent();
        auto alignContentIndex = (int)Utils::findIndex(alignValues, alignContent);
        if (alignContentIndex >= 0)
        {
            if (ImGui::Combo("Align Content", &alignContentIndex, alignLabels))
            {
                element.alignItems(alignValues[alignItemsIndex]);
            }
        }

        auto alignSelf = element.alignSelf();
        auto alignSelfIndex = (int)Utils::findIndex(alignValues, alignSelf);
        if (alignSelfIndex >= 0)
        {
            if (ImGui::Combo("Align Self", &alignSelfIndex, alignLabels))
            {
                element.alignSelf(alignValues[alignSelfIndex]);
            }
        }

        ImGuiYGPaddingAll(element);
        ImGuiYGMarginAll(element);

        if (element.elementType() == "Text")
        {
            auto textElement = dynamic_cast<UITextElement &>(element);
            ImGui::Text("Text [%u]: %s", (uint)textElement.text().size(), textElement.text().c_str());
        }

        if (element.parent())
        {
            std::stringstream label;
            label << "Parent: " << element.parent()->elementType();
            if (ImGui::Button(label.str().c_str()))
            {
                _selectedElement = element.parent();
                DrawDebug::highlightElement = element.parent();
            }
        }
        else
        {
            ImGui::Text("No parent");
        }

        ImGui::Text("Children: %u", (int)element.children().size());

        for (auto child : element.children())
        {
            std::stringstream label;
            label << "- " << child->elementType();
            if (ImGui::Button(label.str().c_str()))
            {
                _selectedElement = child;
                DrawDebug::highlightElement = child;
            }
        }
    }
}