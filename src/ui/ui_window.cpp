#include "ui_window.hpp"

#include <iostream>

namespace space
{
    UIWindow::UIWindow(const std::string &windowName)
        : windowName(windowName), position(100, 100), size(100, 100)
    {

    }
    void UIWindow::draw(Engine &engine)
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
} // namespace space
