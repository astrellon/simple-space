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

            void nineSlice(const NineSlice &value) { _nineSlice = value; }
            NineSlice &nineSlice() { return _nineSlice; }
            const NineSlice &nineSlice() const { return _nineSlice; }

            void setTexture(const sf::Texture *texture, int top, int right, int bottom, int left)
            {
                _nineSlice.texture(texture);
                _nineSlice.border(top, right, bottom, left);
            }

        protected:
            // Methods
            virtual void drawSelf(Engine &engine, RenderCamera &target);

        private:
            // Fields
            NineSlice _nineSlice;

            // Methods
    };
} // space