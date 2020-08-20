#include "ui_debug.hpp"

#include "../engine.hpp"
#include "../effects/transition.hpp"
#include "../game_session.hpp"
#include "../imgui/imgui.h"
#include "../debug/draw_debug.hpp"

namespace space
{
    void UIDebug::draw(Engine &engine)
    {
        if (!DrawDebug::showUIWindow)
        {
            return;
        }

        ImGui::Begin("Debug");
        ImGui::Text("Draw Calls: %d", DrawDebug::glDraw);
        ImGui::Checkbox("Slow", &DrawDebug::slow);
        ImGui::Checkbox("Show Physics", &DrawDebug::showPolygons);

        auto transition = engine.currentSession()->currentTransition();
        if (transition)
        {
            ImGui::Text("Transition: %f", transition->percent(engine.timeSinceStart()));
        }
        else
        {
            ImGui::Text("No transition");
        }
        ImGui::End();
    }
} // namespace space
