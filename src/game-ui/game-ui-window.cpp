#include "game-ui-window.hpp"

#include "game-ui-manager.hpp"
#include "ui-nine-slice-image-element.hpp"
#include "ui-text-element.hpp"

namespace space
{
    GameUIWindow::GameUIWindow(): _backPanel(nullptr), _bodyContainer(nullptr), _header(nullptr)
    {

    }

    void GameUIWindow::init(GameUIManager &uiManager)
    {
        _backPanel = uiManager.createElement<UINineSliceImageElement>();
        _header = uiManager.createElement<UITextElement>();
        _bodyContainer = uiManager.createElement<UIElement>();

        flexDirection(YGFlexDirectionColumn);

        _backPanel->nineSlice(uiManager.defaultBackPanel());
        _backPanel->positionType(YGPositionTypeAbsolute);
        _backPanel->widthPercent(100);
        _backPanel->heightPercent(100);
        addChild(_backPanel);

        _header->text("Game Window");
        _header->widthPercent(100);
        _header->height(24);
        _header->margin(1, 8, 1, 8);
        addChild(_header);

        _bodyContainer->widthPercent(100);
        _bodyContainer->heightPercent(100);
        _bodyContainer->flexGrow(1.0f);
        _bodyContainer->flexShrink(1.0f);
        _bodyContainer->margin(8, 8, 8, 8);
        addChild(_bodyContainer);
    }
} // space