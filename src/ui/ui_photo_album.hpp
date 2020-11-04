#pragma once

#include "ui_window.hpp"

namespace space
{
    class Engine;

    class UIPhotoAlbum : public UIWindow
    {
        public:
            // Fields

            // Constructor
            UIPhotoAlbum();

            // Methods

        protected:
            // Methods
            virtual bool isOpen(Engine &engine);
            virtual void doDraw(Engine &engine);
    };
} // space