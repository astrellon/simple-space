#include "ui_debug.hpp"

#include "../engine.hpp"
#include "../game_session.hpp"
#include "../effects/transition.hpp"
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
        ImGui::Checkbox("Hide GameSession", &DrawDebug::hideGameSession);

        auto transition = engine.currentSession()->currentTransition();
        if (transition)
        {
            ImGui::Text("Transition: %f", transition->percent(engine.timeSinceStart()));
        }
        else
        {
            ImGui::Text("No transition");
        }

        ImGui::Text("Mem: %lu %lu %lu", DrawDebug::totalMemoryAllocated, DrawDebug::allocatedThisFrame, DrawDebug::freedThisFrame);

        ImGui::Text("Locks: %lu", DrawDebug::locksUsed);
        ImGui::Text("Average Frame: %fus", DrawDebug::averageFrameDuration());
        ImGui::PlotLines("Frame Times", DrawDebug::frameDurations.data(), DrawDebug::frameDurations.size());

        if (engine.currentSession() && ImGui::Button("Save"))
        {
            engine.currentSession()->saveGame();
        }

        ImGui::End();
    }
} // namespace space