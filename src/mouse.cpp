#include "mouse.hpp"

#include <algorithm>

namespace space
{
    sf::Vector2i Mouse::_PrevMousePosition;
    Mouse::ButtonList Mouse::_ButtonsUp;
    Mouse::ButtonList Mouse::_ButtonsDown;

    Mouse::ButtonPositions Mouse::_ButtonUpPositions;
    Mouse::ButtonPositions Mouse::_ButtonDownPositions;

    bool Mouse::isMousePressed(sf::Mouse::Button button)
    {
        return sf::Mouse::isButtonPressed(button);
    }

    bool Mouse::isMouseUp(sf::Mouse::Button button)
    {
        return std::find(_ButtonsUp.begin(), _ButtonsUp.end(), button) != _ButtonsUp.end();
    }

    bool Mouse::isMouseDown(sf::Mouse::Button button)
    {
        return std::find(_ButtonsDown.begin(), _ButtonsDown.end(), button) != _ButtonsDown.end();
    }

    void Mouse::resetButtons()
    {
        _ButtonsUp.clear();
        _ButtonsDown.clear();
    }

    void Mouse::setMouseUp(sf::Mouse::Button button)
    {
        _ButtonsUp.push_back(button);
    }

    void Mouse::setMouseDown(sf::Mouse::Button button)
    {
        _ButtonsDown.push_back(button);
    }

    sf::Vector2i Mouse::mouseDownPosition(sf::Mouse::Button button)
    {
        auto find = _ButtonDownPositions.find(button);
        if (find == _ButtonDownPositions.end())
        {
            return sf::Vector2i(-1, -1);
        }

        return find->second;
    }

    sf::Vector2i Mouse::mouseUpPosition(sf::Mouse::Button button)
    {
        auto find = _ButtonUpPositions.find(button);
        if (find == _ButtonUpPositions.end())
        {
            return sf::Vector2i(-1, -1);
        }

        return find->second;
    }

    void Mouse::mouseDownPosition(sf::Mouse::Button button, sf::Vector2i position)
    {
        _ButtonDownPositions[button] = position;
    }
    void Mouse::mouseUpPosition(sf::Mouse::Button button, sf::Vector2i position)
    {
        _ButtonUpPositions[button] = position;
    }

    void Mouse::prevMousePosition(sf::Vector2i position)
    {
        _PrevMousePosition = position;
    }
    sf::Vector2i Mouse::prevMousePosition()
    {
        return _PrevMousePosition;
    }
} // space