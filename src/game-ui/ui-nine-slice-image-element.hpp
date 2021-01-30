#pragma once

#include "ui-element.hpp"
#include "../nine_slice.hpp"

namespace space
{
    class UINineSliceImageElement : public UIElement
    {
        public:
            // Fields

            // Constructor

            // Methods
            virtual ElementType elementType() const { return ElementType::NineSliceImage; }

            NineSlice &nineSlice() { return _nineSlice; }
            const NineSlice &nineSlice() const { return _nineSlice; }

        protected:
            // Methods
            virtual void drawSelf(Engine &engine, RenderCamera &target);

        private:
            // Fields
            NineSlice _nineSlice;

            // Methods
    };
} // space