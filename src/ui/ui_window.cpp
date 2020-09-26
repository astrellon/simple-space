#include "ui_window.hpp"

#include <iostream>

namespace space
{
    UIWindow::UIWindow(const std::string &windowName)
        : windowName(windowName), show(false)
    {
    }

    void UIWindow::draw(Engine &engine)
    {
        if (!isOpen(engine))
        {
            return;
        }

        ImGui::Begin(windowName.c_str());
        doDraw(engine);
        ImGui::End();
    }
} // namespace space
