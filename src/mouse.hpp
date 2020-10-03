#pragma once

#include <vector>
#include <map>

#include <SFML/Window.hpp>

namespace space
{
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

            typedef std::vector<sf::Mouse::Button> ButtonList;
            typedef std::map<sf::Mouse::Button, sf::Vector2i> ButtonPositions;

        private:
            static sf::Vector2i _PrevMousePosition;
            static ButtonList _ButtonsDown;
            static ButtonList _ButtonsUp;

            static ButtonPositions _ButtonDownPositions;
            static ButtonPositions _ButtonUpPositions;

    };
} // space