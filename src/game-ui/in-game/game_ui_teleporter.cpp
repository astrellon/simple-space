#include "game_ui_teleporter.hpp"

#include "../ui_text_element.hpp"
#include "../ui_button.hpp"
#include "../game_ui_manager.hpp"

#include "../../game/items/teleporter.hpp"
#include "../../engine.hpp"
#include "../../game_session.hpp"
#include "../../game/character.hpp"

namespace space
{
    void GameUITeleporter::init(GameUIManager &uiManager)
    {
        _text = uiManager.createElement<UITextElement>();
        _uiManager = &uiManager;

        _text->widthPercent(100);
        _text->flexShrink(1.0f);
        addChild(_text);

        _actionButton = uiManager.createElement<UIButton>();
        _actionButton->width(50);
        _actionButton->text("Go");
        addChild(_actionButton);

        flexDirection(YGFlexDirectionRow);

        _actionButton->onClick([this, &uiManager] (const sf::Event &e)
        {
            if (this->_teleporter.item == nullptr)
            {
                return UIEventResult::Triggered;
            }

            auto session = uiManager.engine().currentSession();
            auto character = session->playerController().controllingCharacter();
            auto placed = this->_teleporter.placed;
            session->moveSpaceObject(static_cast<SpaceObject *>(character), placed->transform().position, placed->insideArea(), true);

            return UIEventResult::Triggered;
        });
    }

    void GameUITeleporter::teleporter(PlacedItemPair<Teleporter> teleporter)
    {
        _teleporter = teleporter;
        _text->text(teleporter.item->name());
    }
} // space