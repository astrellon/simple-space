#include "game_ui_inventory_item.hpp"

#include "../ui_text_element.hpp"
#include "../ui_button.hpp"
#include "../game_ui_manager.hpp"

#include "../../game/items/item.hpp"
#include "../../game/items/placeable_item.hpp"
#include "../../controllers/player_controller.hpp"
#include "../../engine.hpp"
#include "../../game_session.hpp"

namespace space
{
    void GameUIInventoryItem::init(GameUIManager &uiManager)
    {
        _text = uiManager.createElement<UITextElement>();
        _dropButton = uiManager.createElement<UIButton>();

        _text->widthPercent(100);
        _text->flexShrink(1.0f);

        _dropButton->text("Drop");
        _dropButton->width(50);
        _dropButton->onClick([&](const sf::Event &e)
        {
            uiManager.engine().currentSession()->nextFrameState().addAction([&]()
            {
                std::cout << "Drop item!" << std::endl;
                auto placeableItem = dynamic_cast<PlaceableItem *>(this->_item);
                if (placeableItem != nullptr)
                {
                    uiManager.engine().currentSession()->playerController().dropItem(placeableItem);
                }
            });
            return UIEventResult::Triggered;
        });

        flexDirection(YGFlexDirectionRow);

        addChild(_text);
        addChild(_dropButton);
    }

    void GameUIInventoryItem::item(Item *item)
    {
        _item = item;
        _text->text(item->definition.name);

        auto placeableItem = dynamic_cast<PlaceableItem *>(item);
        if (placeableItem != nullptr)
        {
            addChild(_dropButton);
        }
        else
        {
            removeChild(_dropButton);
        }
    }
} // space