#include "ui_panel.hpp"

namespace space
{
    UIPanel::UIPanel(const std::string &windowName)
        : UIWindow(windowName), position(100, 100), size(100, 100)
    {

    }

    void UIPanel::draw(Engine &engine)
    {
        if (!isOpen(engine))
        {
            return;
        }

        checkPosition(engine);
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowPos(position);
        ImGui::Begin(windowName.c_str(), NULL, ImGuiWindowFlags_NoDecoration);
        doDraw(engine);
        ImGui::End();
    }
} // space