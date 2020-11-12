#include "ui_live_photo.hpp"

#include "../imgui/imgui-SFML.h"

#include "../engine.hpp"
#include "../game/live_photo.hpp"

namespace space
{
    void UILivePhoto::draw(Engine &engine, LivePhoto &photo)
    {
        auto &session = *engine.currentSession();
        photo.updateInternalTarget(session, engine.deltaTime());
        photo.drawToInternalTexture(session);

        // Somewhat awkward way to flip the texture upside down.
        auto &texture = photo.texture().getTexture();
        auto size = texture.getSize();
        sf::FloatRect rect(0, size.y, size.x, -(float)size.y);
        ImGui::Image(texture, rect);
    }
} // space