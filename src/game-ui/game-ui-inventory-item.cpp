#include "game-ui-inventory-item.hpp"

#include "../game/items/item.hpp"
#include "../game/items/placeable_item.hpp"
#include "../controllers/player_controller.hpp"
#include "../engine.hpp"
#include "../game_session.hpp"

#include "./ui-text-element.hpp"
#include "./ui-button.hpp"
#include "./game-ui-manager.hpp"

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
        _dropButton->on(sf::Event::EventType::MouseButtonPressed, [&](const sf::Event &e)
        {
            std::cout << "Drop item!" << std::endl;
            auto placeableItem = dynamic_cast<PlaceableItem *>(this->_item);
            if (placeableItem != nullptr)
            {
                uiManager.engine().currentSession()->playerController().dropItem(placeableItem);
            }
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