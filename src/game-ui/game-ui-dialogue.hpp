#pragma once

#include <string>
#include <vector>

#include "./ui-element.hpp"
#include <observe/observer.h>

namespace space
{
    class Dialogue;
    class DialogueManager;
    class Engine;
    class GameUIManager;
    class UITextElement;
    class UIPanel;
    class UIButton;

    class GameUIDialogue : public UIElement
    {
        public:
            // Fields

            // Constructor
            GameUIDialogue();
            ~GameUIDialogue();

            // Methods
            virtual void init(GameUIManager &uiManager);
            virtual void draw(Engine &engine, RenderCamera &target);
            virtual void update(Engine &engine, sf::Time dt, sf::Vector2f parentOffset);

            virtual const char*elementType() const { return "GameUIDialogue"; }

        private:
            // Fields
            GameUIManager *_uiManager;
            std::string _text;
            std::vector<char> _textDisplay;
            size_t _textDisplayIndex;
            UITextElement *_contentText;
            UITextElement *_nameText;
            UIButton *_nextButton;
            UIPanel *_panel;

            observe::Observer _removeDialogueHandler;

            // Methods
            void processText(DialogueManager &manager);
    };
} // space