#pragma once

#include <vector>
#include <memory>

#include <SFML/Window.hpp>
#include <yoga/Yoga.h>

#include "../non_copyable.hpp"
#include "ui-element.hpp"

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

                return result;
            }
            bool removeElement(UIElement *element);

            void processEvent(Engine &engine, const sf::Event &event);

            void update(Engine &engine, sf::Time dt);
            void draw(Engine &engine, RenderCamera &target);

            UIElement *currentHover() { return _currentHover; }
            void currentHover(UIElement *element) { _currentHover = element; }

            UIElement *rootElement() { return _rootElement; }
            void rootElement(UIElement *element);

            YGNodeRef bodyNode() { return _body; }

        private:
            // Fields
            std::vector<std::unique_ptr<UIElement>> _allElements;
            UIElement *_rootElement;
            UIElement *_currentHover;

            YGNodeRef _body;

            // Methods
    };
} // space