#include "ui_editor_menu.hpp"

#include "../../imgui/imgui.h"
#include "../../engine.hpp"

#include "../ui_manager.hpp"

#include "ui_area_selector.hpp"
#include "ui_object_create.hpp"

namespace space
{
    UIEditorMenu::UIEditorMenu() : UIWindow("Editor Menu")
    {

    }

    void UIEditorMenu::draw(Engine &engine)
    {
        if (!isOpen(engine))
        {
            return;
        }

        doDraw(engine);
    }

    void UIEditorMenu::doDraw(Engine &engine)
    {
        if (ImGui::BeginMainMenuBar())
        {
            auto &uiManager = engine.uiManager();
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save"))
                {
                    std::cout << "Save!\n";
                }
                if (ImGui::MenuItem("Load"))
                {
                    std::cout << "Save!\n";
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Quit"))
                {
                    std::cout << "Quit!\n";
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                auto showAreaSelector = uiManager.uiAreaSelector().show;
                if (ImGui::MenuItem("Area Selector", nullptr, showAreaSelector)) { showAreaSelector = !showAreaSelector; }

                auto showObjectCreate = uiManager.uiObjectCreate().show;
                if (ImGui::MenuItem("Object Create", nullptr, showObjectCreate)) { showObjectCreate = !showObjectCreate; }

                uiManager.uiAreaSelector().show = showAreaSelector;
                uiManager.uiObjectCreate().show = showObjectCreate;

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

} // space