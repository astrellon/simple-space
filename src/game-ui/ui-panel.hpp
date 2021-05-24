#pragma once

#include "./ui-element.hpp"

namespace space
{
    class UINineSliceImageElement;

    class UIPanel : public UIElement
    {
        public:
            // Fields

            // Constructor
            UIPanel();

            // Methods
            virtual const char *elementType() const { return "Panel"; }

            virtual void init(GameUIManager &uiManager);

            UINineSliceImageElement *backPanel() { return _backPanel; }
            UIElement *bodyContainer() { return _bodyContainer; }

        protected:
            // Fields
            UINineSliceImageElement *_backPanel;
            UIElement *_bodyContainer;

            // Methods
    };
} // space