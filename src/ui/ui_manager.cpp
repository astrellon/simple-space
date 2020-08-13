#include "ui_manager.hpp"

#include "ui_window.hpp"

namespace space
{
    void UIManager::draw(Engine &engine)
    {
        for (auto &window : _uiWindows)
        {
            window->draw(engine);
        }
    }
} // namespace space
