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

    class GameUIInventoryWindow;
    class GameUIInteractablesPanel;
    class GameUITeleportersPanel;

    class GameUIManager : private NonCopyable
    {
        public:
            // Fields

            // Constructor
            GameUIManager(Engine &engine);

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

            inline Engine &engine() { return _engine; }

            void processEvent(const sf::Event &event);

            void preUpdate(sf::Time dt);
            void update(sf::Time dt);
            void draw(RenderCamera &target);

            void checkForMouse(sf::Vector2f mousePosition);
            void updateUnderMouse(sf::Vector2f mousePosition);

            const std::vector<UIElement *> &currentHoverPath() const { return _currentHoverPath; }

            void defaultFont(const sf::Font *font) { _defaultFont = font; }
            const sf::Font *defaultFont() const { return _defaultFont; }

            NineSlice defaultWindowBackPanel() { return _defaultWindowBackPanel; }
            void defaultWindowBackPanel(const NineSlice &value) { _defaultWindowBackPanel = value; }

            NineSlice defaultPanelBackPanel() { return _defaultPanelBackPanel; }
            void defaultPanelBackPanel(const NineSlice &value) { _defaultPanelBackPanel = value; }

            NineSlice defaultButton() { return _defaultButton; }
            void defaultButton(const NineSlice &value) { _defaultButton = value; }

            UIElement *body() { return _bodyElement; }

            const std::vector<std::unique_ptr<UIElement>> &allElements() const { return _allElements; }

            void initDefaultWindows();

            GameUIInteractablesPanel &interactablesPanel() { return *_interactablesPanel; }
            GameUITeleportersPanel &teleportersPanel() { return *_teleportersPanel; }
            GameUIInventoryWindow &inventoryWindow() { return *_inventoryWindow; }

        private:
            // Fields
            Engine &_engine;
            std::vector<std::unique_ptr<UIElement>> _allElements;
            std::vector<UIElement *> _currentHoverPath;
            std::vector<UIElement *> _previousHoverPath;
            UIRootElement *_bodyElement;

            const sf::Font *_defaultFont;
            NineSlice _defaultWindowBackPanel;
            NineSlice _defaultPanelBackPanel;
            NineSlice _defaultButton;

            GameUIInteractablesPanel *_interactablesPanel;
            GameUITeleportersPanel *_teleportersPanel;
            GameUIInventoryWindow *_inventoryWindow;

            // Methods
    };
} // space