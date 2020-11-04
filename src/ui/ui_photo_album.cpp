#include "ui_photo_album.hpp"

#include "../imgui/imgui-SFML.h"

#include "../engine.hpp"
#include "../game_session.hpp"
#include "../photo_album.hpp"
#include "../game/live_photo.hpp"
#include "../controllers/player_controller.hpp"

namespace space
{
    UIPhotoAlbum::UIPhotoAlbum() : UIWindow("PhotoAlbum")
    {

    }

    bool UIPhotoAlbum::isOpen(Engine &engine)
    {
        if (!engine.currentSession())
        {
            return false;
        }

        auto &player = engine.currentSession()->playerController();
        return show && !engine.currentSession()->dialogueManager().isInDialogue();
    }

    void UIPhotoAlbum::doDraw(Engine &engine)
    {
        auto &session = *engine.currentSession();
        auto &album = session.playerController().photoAlbum();

        ImGui::Text("Photos: %i", (int)album.photos().size());

        for (auto photo : album.photos())
        {
            ImGui::Text("- Photo %s", photo->id.c_str());
            photo->updateInternalTarget(session, engine.deltaTime());
            photo->drawToInternalTexture(session);

            // Somewhat awkward way to flip the texture upside down.
            auto &texture = photo->texture().getTexture();
            auto size = texture.getSize();
            sf::FloatRect rect(0, size.y, size.x, -(float)size.y);
            ImGui::Image(texture, rect);
        }
    }
} // space