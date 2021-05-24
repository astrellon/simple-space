#include "ui-panel.hpp"

#include "./game-ui-manager.hpp"
#include "./ui-nine-slice-image-element.hpp"

namespace space
{
    UIPanel::UIPanel(): _bodyContainer(nullptr), _backPanel(nullptr)
    {

    }

    void UIPanel::init(GameUIManager &uiManager)
    {
        UIElement::init(uiManager);

        _backPanel = uiManager.createElement<UINineSliceImageElement>();
        _bodyContainer = uiManager.createElement<UIElement>();

        _backPanel->nineSlice(uiManager.defaultPanelBackPanel());
        _backPanel->positionType(YGPositionTypeAbsolute);
        _backPanel->widthPercent(100);
        _backPanel->heightPercent(100);
        addChild(_backPanel);

        _bodyContainer->widthPercent(100);
        _bodyContainer->heightPercent(100);
        _bodyContainer->flexGrow(1.0f);
        _bodyContainer->flexShrink(1.0f);
        _bodyContainer->padding(8, 8, 8, 8);
        addChild(_bodyContainer);

        widthAuto();
        heightAuto();
    }
} // space