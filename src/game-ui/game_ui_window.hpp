#pragma once

#include "./ui_element.hpp"

namespace space
{
    class UINineSliceImageElement;
    class UITextElement;
    class GameUIManager;

    class GameUIWindow : public UIElement
    {
        public:
            // Fields

            // Constructor
            GameUIWindow();

            // Methods
            virtual void init(GameUIManager &uiManager);

            UINineSliceImageElement *backPanel() { return _backPanel; }
            UITextElement *header() { return _header; }
            UIElement *bodyContainer() { return _bodyContainer; }

            virtual const char*elementType() const { return "GameUIWindow"; }

        private:
            // Fields
            UINineSliceImageElement *_backPanel;
            UITextElement *_header;
            UIElement *_bodyContainer;

            // Methods
    };
} // space