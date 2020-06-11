#pragma once

#include <vector>

#include <SFML/Window.hpp>

namespace space
{
    class Keyboard
    {
        public:
            static bool isKeyPressed(sf::Keyboard::Key key);

            static bool isKeyDown(sf::Keyboard::Key key);

            static bool isKeyUp(sf::Keyboard::Key key);

            static void resetKeys();
            static void setKeyDown(sf::Keyboard::Key key);

            static void setKeyUp(sf::Keyboard::Key key);

            typedef std::vector<sf::Keyboard::Key> KeyList;

        private:
            static KeyList _KeysDown;
            static KeyList _KeysUp;
    };
} // town