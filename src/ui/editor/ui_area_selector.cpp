#include "ui_area_selector.hpp"

#include <sstream>

#include "../../engine.hpp"
#include "../../game_session.hpp"
#include "../../editor_game_session.hpp"
#include "../../game/area.hpp"
#include "../../game/ihas_area.hpp"

namespace space
{
    UIAreaSelector::UIAreaSelector() : UIWindow("Area Selector")
    {

    }

    void UIAreaSelector::doDraw(Engine &engine)
    {
        auto session = engine.currentSession();
        if (!session)
        {
            return;
        }

        auto &areaWithObjects = session->objectsWithArea();
        for (auto hasArea : areaWithObjects)
        {
            auto obj = dynamic_cast<SpaceObject *>(hasArea);

            std::stringstream label;
            label << toString(obj->type()) << ": " << obj->id;

            if (ImGui::Button(label.str().c_str()))
            {
                auto editor = dynamic_cast<EditorGameSession *>(engine.currentSession());
                if (editor)
                {
                    editor->moveCameraTo(hasArea->area());
                }
            }
        }
    }
} // space