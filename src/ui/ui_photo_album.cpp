#include "ui_photo_album.hpp"

#include "../imgui/imgui-SFML.h"

#include "../engine.hpp"
#include "../game_session.hpp"
#include "../photo_album.hpp"
#include "../game/live_photo.hpp"
#include "../controllers/player_controller.hpp"

#include "./ui_live_photo.hpp"

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
        auto &session = *engine.currentSession();

        auto &player = session.playerController();
        return show && !session.dialogueManager().isInDialogue() && !session.isTakingAPhoto();
    }

    void UIPhotoAlbum::doDraw(Engine &engine)
    {
        auto &session = *engine.currentSession();
        auto &album = session.playerController().photoAlbum();

        ImGui::Text("Photos: %i", (int)album.photos().size());

        if (ImGui::Button("Take a photo!"))
        {
            session.takingAPhoto(true);
        }

        for (auto photo : album.photos())
        {
            ImGui::Text("- Photo %s", photo->id.c_str());
            UILivePhoto::draw(engine, *photo);
        }
    }
} // space