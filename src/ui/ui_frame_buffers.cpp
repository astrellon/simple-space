#include "ui_frame_buffers.hpp"

#include <sstream>
#include "../imgui/imgui-SFML.h"

#include "../engine.hpp"

namespace space
{
    UIFrameBuffers::UIFrameBuffers() : UIWindow("FrameBuffers")
    {

    }

    void UIFrameBuffers::doDraw(Engine &engine)
    {
        auto &renderCameras = engine.renderCameras();
        ImGui::Text("Frame Buffers: %i", (int)renderCameras.objects().size());

        for (auto &renderCamera : renderCameras.objects())
        {
            ImGui::Text("- %s", renderCamera->camera().debugName.c_str());
            ImGui::Image(renderCamera->texture().getTexture());
        }
    }
} // space