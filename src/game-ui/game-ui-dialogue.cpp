#include "game-ui-dialogue.hpp"

#include "./game-ui-manager.hpp"
#include "./ui-text-element.hpp"
#include "./ui-panel.hpp"
#include "./ui-button.hpp"

#include "../dialogue_manager.hpp"
#include "../engine.hpp"
#include "../game_session.hpp"

#include "../mustache.hpp"

using kainjow::mustache::mustache;

namespace space
{
    GameUIDialogue::GameUIDialogue() : _textDisplayIndex(0), _contentText(nullptr), _nameText(nullptr), _uiManager(nullptr)
    {

    }

    GameUIDialogue::~GameUIDialogue()
    {
        _removeDialogueHandler.reset();
    }

    void GameUIDialogue::init(GameUIManager &uiManager)
    {
        UIElement::init(uiManager);
        _uiManager = &uiManager;

        _panel = uiManager.createElement<UIPanel>();
        addChild(_panel);

        _nameText = uiManager.createElement<UITextElement>();
        _panel->bodyContainer()->addChild(_nameText);

        _nextButton = uiManager.createElement<UIButton>();
        _panel->bodyContainer()->addChild(_nextButton);

        _contentText = uiManager.createElement<UITextElement>();
        _panel->bodyContainer()->addChild(_contentText);

        _panel->maxWidth(500);
        _panel->flexGrow(1);

        _nextButton->positionType(YGPositionTypeAbsolute);
        _nextButton->position(YGEdgeBottom, 20);
        _nextButton->position(YGEdgeRight, 20);
        _nextButton->width(60);
        _nextButton->text("Next");

        auto &manager = uiManager.engine().currentSession()->dialogueManager();
        _removeDialogueHandler = manager.onNextDialogue.createObserver([this, &manager]()
        {
            this->processText(manager);
        });

        _nextButton->on(sf::Event::EventType::MouseButtonReleased, [&manager]( const sf::Event &e)
        {
            manager.nextLine();

            return UIEventResult::Triggered;
        });

        height(128);
        widthPercent(100);
        justifyContent(YGJustifyCenter);
        flexDirection(YGFlexDirectionRow);
        positionType(YGPositionTypeAbsolute);
        position(YGEdgeBottom, 20);
    }

    void GameUIDialogue::draw(Engine &engine, RenderCamera &target)
    {
        auto &manager = engine.currentSession()->dialogueManager();
        if (!manager.isInDialogue())
        {
            return;
        }

        UIElement::draw(engine, target);
    }

    void GameUIDialogue::update(Engine &engine, sf::Time dt, sf::Vector2f parentOffset)
    {
        auto &manager = engine.currentSession()->dialogueManager();
        if (!manager.isInDialogue())
        {
            return;
        }

        UIElement::update(engine, dt, parentOffset);

        if (_textDisplayIndex < _text.size())
        {
            _textDisplay[_textDisplayIndex] = _text[_textDisplayIndex];
            _textDisplayIndex++;
            _textDisplay[_textDisplayIndex] = '\0';

            _contentText->text(_textDisplay.data());
        }
    }

    void GameUIDialogue::processText(DialogueManager &manager)
    {
        mustache tmplt{ manager.currentLine() };
        _text = tmplt.render({"name", manager.personTalkingName()});
        _textDisplayIndex = 0;
        _textDisplay.reserve(_text.size() + 1);
        _textDisplay[0] = '\0';

        _nameText->text(manager.personTalkingName());
        _contentText->text("");
    }
} // space