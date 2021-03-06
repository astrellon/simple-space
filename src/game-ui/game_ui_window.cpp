#include "game_ui_window.hpp"

#include "game_ui_manager.hpp"
#include "ui_nine_slice_image_element.hpp"
#include "ui_text_element.hpp"

namespace space
{
    GameUIWindow::GameUIWindow():
        _backPanel(nullptr),
        _header(nullptr),
        _bodyContainer(nullptr)
    {

    }

    void GameUIWindow::init(GameUIManager &uiManager)
    {
        _backPanel = uiManager.createElement<UINineSliceImageElement>();
        _header = uiManager.createElement<UITextElement>();
        _bodyContainer = uiManager.createElement<UIElement>();

        flexDirection(YGFlexDirectionColumn);

        _backPanel->nineSlice(uiManager.defaultWindowBackPanel());
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
        _bodyContainer->padding(8, 8, 8, 8);
        addChild(_bodyContainer);
    }
} // space