#include "ui_debug.hpp"

#include "../engine.hpp"
#include "ui_manager.hpp"
#include "../game_session.hpp"
#include "../effects/transition.hpp"
#include "../imgui/imgui.h"
#include "../debug/draw_debug.hpp"

namespace space
{
    UIDebug::UIDebug() : UIWindow("Debug")
    {

    }

    void UIDebug::doDraw(Engine &engine)
    {
        ImGui::Text("Draw Calls: %d", DrawDebug::glDraw);

        if (ImGui::TreeNode("UI Windows"))
        {
            for (auto &uiWindow : engine.uiManager().windows())
            {
                if (ImGui::TreeNode((void *)uiWindow.get(), "%s", uiWindow->windowName.c_str()))
                {
                    ImGui::Checkbox("Show", &uiWindow->show);

                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        ImGui::Checkbox("Slow", &DrawDebug::slow);
        ImGui::Checkbox("Show Physics", &DrawDebug::showPolygons);
        ImGui::Checkbox("Hide GameSession", &DrawDebug::hideGameSession);
        ImGui::Checkbox("Hide Everything", &DrawDebug::hideEverything);
        ImGui::Checkbox("Hide Background", &DrawDebug::hideBackground);
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

        ImGui::SliderInt("Font Size", &DrawDebug::fontSize, 4, 72);

        ImGui::Text("Render Resolution: %u x %u", engine.renderSize().x, engine.renderSize().y);
        ImGui::Text("Window Resolution: %u x %u", engine.windowSize().x, engine.windowSize().y);
        ImGui::SliderFloat("FOV", &DrawDebug::fov, -1, 6);

        if (engine.currentSession() && ImGui::Button("Save"))
        {
            engine.currentSession()->saveGame();
        }
    }
} // namespace space
