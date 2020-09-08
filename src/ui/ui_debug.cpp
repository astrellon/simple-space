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
        ImGui::Checkbox("Hide Everything", &DrawDebug::hideEverything);
        ImGui::Checkbox("Bloom", &engine.enableBloom);
        ImGui::Checkbox("Show Portal Shapes", &DrawDebug::showPortalShapes);

        auto transition = engine.currentSession()->currentTransition();
        if (transition)
        {
            ImGui::Text("Transition: %f", transition->percent(engine.timeSinceStart()));
        }
        else
        {
            ImGui::Text("No transition");
        }

#ifdef TRACK_MEMORY
        ImGui::Text("Mem: %lu %lu %lu %lu", DrawDebug::totalMemoryAllocated, DrawDebug::allocatedThisFrame, DrawDebug::freedThisFrame, DrawDebug::numAllocations);
#else
        ImGui::Text("Memory tracking disabled");
#endif

        ImGui::Text("Locks: %lu", DrawDebug::locksUsed);

        auto averageFrameDuration = DrawDebug::averageFrameDuration();
        ImGui::Text("Average Frame: %.1fus %.1ffps", averageFrameDuration, 1000000.0f / averageFrameDuration);
        ImGui::PlotLines("Frame Times", DrawDebug::frameDurations.data(), DrawDebug::frameDurations.size());

        if (engine.currentSession() && ImGui::Button("Save"))
        {
            engine.currentSession()->saveGame();
        }

        ImGui::End();
    }
} // namespace space
