#pragma once

#include "../ui_panel.hpp"

namespace space
{
    class UISelected : public UIPanel
    {
        public:
            // Constructors
            UISelected();

        protected:
            // Methods
            virtual bool isOpen(Engine &engine);
            virtual void checkPosition(Engine &engine);
            virtual void doDraw(Engine &engine);

        private:
            // Fields

            // Methods

    };
} // space