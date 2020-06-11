#include "keyboard.hpp"

#include <algorithm>

namespace space
{
    Keyboard::KeyList Keyboard::_KeysUp;
    Keyboard::KeyList Keyboard::_KeysDown;

    bool Keyboard::isKeyPressed(sf::Keyboard::Key key)
    {
        return sf::Keyboard::isKeyPressed(key);
    }

    bool Keyboard::isKeyUp(sf::Keyboard::Key key)
    {
        return std::find(_KeysUp.begin(), _KeysUp.end(), key) != _KeysUp.end();
    }

    bool Keyboard::isKeyDown(sf::Keyboard::Key key)
    {
        return std::find(_KeysDown.begin(), _KeysDown.end(), key) != _KeysDown.end();
    }

    void Keyboard::resetKeys()
    {
        _KeysUp.clear();
        _KeysDown.clear();
    }

    void Keyboard::setKeyUp(sf::Keyboard::Key key)
    {
        _KeysUp.push_back(key);
    }

    void Keyboard::setKeyDown(sf::Keyboard::Key key)
    {
        _KeysDown.push_back(key);
    }
}