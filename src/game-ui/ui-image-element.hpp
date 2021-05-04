#pragma once

#include "ui-element.hpp"
#include "../animated_sprite.hpp"

namespace space
{
    class UIImageElement : public UIElement
    {
        public:
            // Fields

            // Constructor

            // Methods
            virtual const char *elementType() const { return "Image"; }

        private:
            // Fields

            // Methods
    };
} // space