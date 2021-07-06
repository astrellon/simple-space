#include "game_ui_stop_flying.hpp"

#include "../game_ui_manager.hpp"
#include "../ui_button.hpp"

#include "../../engine.hpp"
#include "../../game_session.hpp"

namespace space
{
    void GameUIStopFlying::init(GameUIManager &uiManager)
    {
        positionType(YGPositionTypeAbsolute);
        widthPercent(100);
        alignItems(YGAlignCenter);

        _button = uiManager.createElement<UIButton>();
        addChild(_button);

        _button->text("Stop Flying");
        _button->width(80);

        _button->onClick([&uiManager](const sf::Event &e)
        {
            uiManager.engine().currentSession()->setPlayerControllingCharacter();
            return UIEventResult::Triggered;
        });
    }

    void GameUIStopFlying::preUpdate(Engine &engine, sf::Time dt)
    {
        auto gameSession = engine.currentSession();
        if (!gameSession)
        {
            return;
        }

        if (gameSession->playerController().controlling() != ControllingValue::ControlShip)
        {
            visible(false);
        }
        else
        {
            visible(true);

            auto screenSize = engine.renderSize();
            // margin(, 0, 0, screenSize.x / 2 + 32);
            margin(screenSize.y - 32, 0, 0, 0);
        }
    }
} // space