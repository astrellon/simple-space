#pragma once

#include <vector>

#include "ui_panel.hpp"

namespace space
{
    class Ship;
    class Planet;
    class CharacterController;

    class UITeleporter : public UIPanel
    {
        public:
            // Fields

            // Constructor
            UITeleporter();

            // Methods
            void controller(CharacterController *controller) { _characterController = controller; }
            CharacterController *controller() const { return _characterController; }

        protected:
            // Methods
            virtual bool isOpen(Engine &engine);
            virtual void checkPosition(Engine &engine);
            virtual void doDraw(Engine &engine);

        private:
            // Fields
            CharacterController *_characterController;

    };
} // space