#include "ui_compendium.hpp"

#include "../imgui/imgui-SFML.h"

#include "../engine.hpp"
#include "../game_session.hpp"
#include "../compendium.hpp"
#include "../controllers/player_controller.hpp"
#include "../definitions/compendium_definition.hpp"

#include "./ui_live_photo.hpp"

namespace space
{
    UICompendium::UICompendium() : UIWindow("Compendium")
    {

    }

    bool UICompendium::isOpen(Engine &engine)
    {
        if (!engine.currentSession())
        {
            return false;
        }
        auto &session = *engine.currentSession();

        auto &player = session.playerController();
        return show && !session.dialogueManager().isInDialogue() && !session.isTakingAPhoto();
    }

    void UICompendium::doDraw(Engine &engine)
    {
        auto &session = *engine.currentSession();
        auto &compendium = session.playerController().compendium();

        ImGui::Text("Compendium Entry: %i", (int)compendium.entries().size());

        for (auto entryKvp : compendium.entries())
        {
            auto &compDef = entryKvp.second.definition;
            ImGui::Text("- Entry: %s", compDef.name.c_str());
            ImGui::Text("- Description: %s", compDef.description.c_str());

            for (auto photo : entryKvp.second.photos())
            {
                UILivePhoto::draw(engine, *photo);
                ImGui::SameLine();
            }

            ImGui::NewLine();
        }
    }
} // space