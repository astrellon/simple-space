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
        float timeScale = engine.timeScale();
        if (ImGui::SliderFloat("Time Scale", &timeScale, 0, 2.0f))
        {
            engine.timeScale(timeScale);
        }
        if (ImGui::Button("Reset Time Scale"))
        {
            engine.timeScale(1.0f);
        }
        ImGui::Checkbox("Pause", &DrawDebug::pauseGame);
        ImGui::Checkbox("Show Physics", &DrawDebug::showPolygons);
        ImGui::Checkbox("Hide GameSession", &DrawDebug::hideGameSession);
        ImGui::Checkbox("Hide Everything", &DrawDebug::hideEverything);
        ImGui::Checkbox("Hide Background", &DrawDebug::hideBackground);
        ImGui::Checkbox("Hide GameUI", &DrawDebug::hideGameUI);
        ImGui::Checkbox("Bloom", &engine.enableBloom);
        ImGui::Checkbox("Show Portal Shapes", &DrawDebug::showPortalShapes);

        auto transition = engine.currentSession() ? engine.currentSession()->currentTransition() : nullptr;
        if (transition)
        {
            ImGui::Text("Transition: %f", transition->percent(engine.inGameTime()));
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
        ImGui::SliderFloat("FOV", &DrawDebug::fov, 0, 1);
        ImGui::SliderFloat("Draw Close", &DrawDebug::drawClose, 0, 200);
        ImGui::SliderFloat("Draw Far", &DrawDebug::drawFar, 1, 1000);

        if (engine.currentSession() && ImGui::Button("Save"))
        {
            engine.currentSession()->saveGame();
        }
    }
} // namespace space
