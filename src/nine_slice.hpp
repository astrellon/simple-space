#pragma once

#include <array>
#include <SFML/Graphics.hpp>

#define BORDER_TOP 0
#define BORDER_RIGHT 1
#define BORDER_BOTTOM 2
#define BORDER_LEFT 3

namespace space
{
    class NineSlice : public sf::Drawable
    {
        public:
            // Fields

            // Constructor
            NineSlice();
            NineSlice(const sf::Texture *texture, int top, int right, int bottom, int left);

            // Methods
            inline void texture(const sf::Texture *texture) { _texture = texture; _dirty = true; }
            inline const sf::Texture *texture() const { return _texture; }

            void border(int top, int right, int bottom, int left)
            {
                _border[BORDER_TOP] = top;
                _border[BORDER_RIGHT] = right;
                _border[BORDER_BOTTOM] = bottom;
                _border[BORDER_LEFT] = left;
                _dirty = true;
            }
            inline const std::array<int, 4> &border() const { return _border; }

            inline void borderTop(int border) { _border[BORDER_TOP] = border; _dirty = true; }
            inline int borderTop() const { return _border[BORDER_TOP]; }

            inline void borderRight(int border) { _border[BORDER_RIGHT] = border; _dirty = true; }
            inline int borderRight() const { return _border[BORDER_RIGHT]; }

            inline void borderBottom(int border) { _border[BORDER_BOTTOM] = border; _dirty = true; }
            inline int borderBottom() const { return _border[BORDER_BOTTOM]; }

            inline void borderLeft(int border) { _border[BORDER_LEFT] = border; _dirty = true; }
            inline int borderLeft() const { return _border[BORDER_LEFT]; }

            inline void width(int width)
            {
                if (width != _dims.x)
                {
                    _dims.x = width; _dirty = true;
                }
            }
            inline int width() const { return _dims.x; }

            inline void height(int height)
            {
                if (height != _dims.y)
                {
                    _dims.y = height; _dirty = true;
                }
            }
            inline int height() const { return _dims.y; }

        protected:
            // Methods
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:
            // Fields
            const sf::Texture *_texture;
            std::array<int, 4> _border;
            sf::Vector2i _dims;

            mutable bool _dirty;
            mutable sf::VertexArray _vertexArray;

            // Methods
            void updateVertices() const;
    };
} // space