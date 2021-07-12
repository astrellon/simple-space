#pragma once

#include <memory>

#include "ui_window.hpp"
#include "../types.hpp"
#include "../game/area.hpp"
#include "ui_object_edit.hpp"

namespace space
{
    class Engine;
    class SpaceObject;

    class UIObjects : public UIWindow
    {
        public:
            // Fields
            ObjectId selectedObjectId;

            // Constructor
            UIObjects();

            // Methods

        protected:
            // Methods
            virtual void doDraw(Engine &engine);

        private:
            // Fields
            std::unique_ptr<UIObjectEdit> _selectedObjectEditor;

            // Methods
    };
} // space