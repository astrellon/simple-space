#pragma once

#include <vector>
#include <memory>

#include <SFML/Window.hpp>
#include <yoga/Yoga.h>

#include "../non_copyable.hpp"
#include "ui-element.hpp"
#include "../nine_slice.hpp"

namespace space
{
    class Engine;
    class RenderCamera;
    class UITextElement;

    class GameUIManager : private NonCopyable
    {
        public:
            // Fields

            // Constructor
            GameUIManager();

            // Methods
            template <typename T, typename... TArgs>
            auto createElement(TArgs &&... args)
            {
                auto obj = std::make_unique<T>(std::forward<TArgs>(args)...);
                auto result = obj.get();
                _allElements.emplace_back(std::move(obj));
                result->init(*this);

                return result;
            }
            bool removeElement(UIElement *element);

            void processEvent(Engine &engine, const sf::Event &event);

            void update(Engine &engine, sf::Time dt);
            void draw(Engine &engine, RenderCamera &target);

            void checkForMouse(Engine &engine, sf::Vector2f mousePosition);
            std::vector<UIElement *> findElementUnderMouse(Engine &engine, sf::Vector2f mousePosition) const;

            UIElement *currentHover() { return _currentHover; }
            void currentHover(UIElement *element) { _currentHover = element; }

            void defaultFont(const sf::Font *font) { _defaultFont = font; }
            const sf::Font *defaultFont() const { return _defaultFont; }

            NineSlice defaultBackPanel() { return _defaultBackPanel; }
            void defaultBackPanel(const NineSlice &value) { _defaultBackPanel = value; }

            UIElement *body() { return _bodyElement; }

            const std::vector<std::unique_ptr<UIElement>> &allElements() const { return _allElements; }

        private:
            // Fields
            std::vector<std::unique_ptr<UIElement>> _allElements;
            UIElement *_currentHover;
            UIRootElement *_bodyElement;

            const sf::Font *_defaultFont;
            NineSlice _defaultBackPanel;

            // Methods
    };
} // space