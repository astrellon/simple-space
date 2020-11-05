#pragma once

#include <string>
#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

namespace space
{
    class Engine;
    class RenderCamera;
    class Cursor;

    class Mouse
    {
        public:
            static bool isMousePressed(sf::Mouse::Button button);
            static bool isMouseDown(sf::Mouse::Button button);
            static bool isMouseUp(sf::Mouse::Button button);

            static void resetButtons();
            static void setMouseUp(sf::Mouse::Button button);
            static void setMouseDown(sf::Mouse::Button button);

            static sf::Vector2i mouseDownPosition(sf::Mouse::Button button);
            static sf::Vector2i mouseUpPosition(sf::Mouse::Button button);

            static void mouseDownPosition(sf::Mouse::Button button, sf::Vector2i position);
            static void mouseUpPosition(sf::Mouse::Button button, sf::Vector2i position);

            static void prevMousePosition(sf::Vector2i position);
            static sf::Vector2i prevMousePosition();

            static void cursor(const Cursor *cursor);
            static const Cursor *cursor() { return _Cursor; }

            static void update(Engine &engine, sf::Time dt);
            static void draw(Engine &engine, RenderCamera &target);

            typedef std::vector<sf::Mouse::Button> ButtonList;
            typedef std::map<sf::Mouse::Button, sf::Vector2i> ButtonPositions;

        private:
            static sf::Vector2i _PrevMousePosition;
            static ButtonList _ButtonsDown;
            static ButtonList _ButtonsUp;

            static ButtonPositions _ButtonDownPositions;
            static ButtonPositions _ButtonUpPositions;

            static sf::Sprite _CursorSprite;
            static const Cursor *_Cursor;

    };
} // space