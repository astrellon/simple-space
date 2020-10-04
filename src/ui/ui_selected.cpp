#include "ui_selected.hpp"

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include "../engine.hpp"
#include "../game_session.hpp"
#include "../controllers/player_controller.hpp"

namespace space
{
    UISelected::UISelected() : UIPanel("Selected")
    {
        size = ImVec2(200, 120);
        position = ImVec2(20, 20);
        show = true;
    }

    bool UISelected::isOpen(Engine &engine)
    {
        if (!engine.currentSession())
        {
            return false;
        }

        return show && engine.currentSession()->playerController().selectedObject().size() > 0;
    }

    void UISelected::checkPosition(Engine &engine)
    {
        auto renderSize = engine.renderSize();
        position.x = (renderSize.x - size.x - 20);
    }

    void UISelected::doDraw(Engine &engine)
    {
        ImGui::Text("Selected");

        auto session = engine.currentSession();
        if (!session)
        {
            return;
        }

        auto id = session->playerController().selectedObject();
        SpaceObject *obj;
        if (!session->tryGetSpaceObject(id, &obj))
        {
            ImGui::Text("Cannot find %s", id.c_str());
            return;
        }

        ImGui::Text("%s: %s", obj->id.c_str(), obj->type().c_str());
    }
} // space