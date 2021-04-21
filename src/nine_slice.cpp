#include "nine_slice.hpp"

#include "./render_camera.hpp"
#include "./utils.hpp"

const std::array<int, 4> empty{0, 0, 0, 0};

namespace space
{
    NineSlice::NineSlice() : _dirty(true), _texture(nullptr), _border(empty), _vertexArray(sf::TrianglesStrip, 24)
    {

    }

    NineSlice::NineSlice(const sf::Texture *texture, int top, int right, int bottom, int left) : _dirty(true), _texture(texture), _border(empty), _vertexArray(sf::TrianglesStrip, 24)
    {
        border(top, right, bottom, left);
    }

    void NineSlice::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (_dirty)
        {
            updateVertices();
            _dirty = false;
        }

        if (_texture)
        {
            states.texture = _texture;
            target.draw(_vertexArray, states);
        }
    }

    void NineSlice::updateVertices() const
    {
        // http://dan.lecocq.us/wordpress/wp-content/uploads/2009/12/strip.png
        //     i ->
        //   j  0     1    2    3
        //   |
        //   V  4     5    6    7
        //
        //      8     9    10   11
        //
        //     12    13    14   15
        //
        // =>       0, 4, 1, 5, 2, 6, 3, 7, 7, 11, 6, 10, 5, 9 ,4 ,8, 8 ,12 ,9 ,13, 10 ,14, 11,15

        const int triangleStripVextexCount = 24;
        const int triangleStripVertexOrder[triangleStripVextexCount] = {0, 4, 1, 5, 2, 6, 3, 7, 7, 11, 6, 10, 5, 9, 4, 8, 8, 12, 9, 13, 10, 14, 11, 15};

        if (_texture)
        {
            int x_pos[4] = {0, borderLeft(), width() - borderRight(), width()};
            int y_pos[4] = {0, borderTop(), height() - borderBottom(), height()};
            sf::Vector2f verticesPosition[16];

            int t_width = _texture->getSize().x;
            int t_height = _texture->getSize().y;

            int x_textCoord[4] = {0, borderLeft(), t_width - borderRight(), t_width};
            int y_textCoord[4] = {0, borderTop(), t_height - borderBottom(), t_height};
            sf::Vector2f verticesTextCoord[16];

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    verticesPosition[i + 4 * j] = sf::Vector2f(x_pos[i], y_pos[j]);
                    verticesTextCoord[i + 4 * j] = sf::Vector2f(x_textCoord[i], y_textCoord[j]);
                }
            }

            for (int i = 0; i < triangleStripVextexCount; i++)
            {
                _vertexArray[i].position = verticesPosition[triangleStripVertexOrder[i]];
                _vertexArray[i].texCoords = verticesTextCoord[triangleStripVertexOrder[i]];
            }
        }
    }
} // space