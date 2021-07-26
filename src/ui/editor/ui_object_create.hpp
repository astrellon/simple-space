#pragma once

#include "../ui_window.hpp"

namespace space
{
    class UIObjectCreate : public UIWindow
    {
        public:
            // Fields

            // Constructor
            UIObjectCreate();

            // Methods

        protected:
            // Methods
            virtual void doDraw(Engine &engine);

        private:
            // Fields
            int _objectIndex;
            int _itemTypeIndex;
            bool _creatingObject;

            // Methods
    };
} // space