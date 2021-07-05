#include "game_ui_dialogue.hpp"

#include <SFML/Audio.hpp>

#include "../game_ui_manager.hpp"
#include "../ui_text_element.hpp"
#include "../ui_panel.hpp"
#include "../ui_button.hpp"

#include "../../dialogue_manager.hpp"
#include "../../engine.hpp"
#include "../../game_session.hpp"

#include "../../definitions/dialogue_audio.hpp"

#include "../../mustache.hpp"

using kainjow::mustache::mustache;

namespace space
{
    GameUIDialogue::GameUIDialogue() :
        _uiManager(nullptr),
        _textDisplayIndex(0),
        _contentText(nullptr),
        _nameText(nullptr),
        _nextButton(nullptr),
        _panel(nullptr),
        _dialogueBlip(std::make_unique<sf::Sound>()),
        _dialogueBlipCooldown(0.0f)
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

        auto &engine = this->_uiManager->engine();
        engine.onGameSceneEnded.createObserver([this](BaseGameScene *gameScene)
        {
            this->_removeDialogueHandler.reset();
        });

        engine.onGameSceneStarted.createObserver([this, &engine](BaseGameScene *gameScene)
        {
            this->onGameSessionStart();
        });
        onGameSessionStart();

        _nextButton->onClick([this, &engine]( const sf::Event &e)
        {
            if (engine.currentSession())
            {
                engine.currentSession()->dialogueManager().nextLine();
            }
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
        if (!engine.currentSession())
        {
            return;
        }

        auto &manager = engine.currentSession()->dialogueManager();
        if (!manager.isInDialogue())
        {
            return;
        }

        UIElement::draw(engine, target);
    }

    void GameUIDialogue::update(Engine &engine, sf::Time dt, sf::Vector2f parentOffset)
    {
        if (!engine.currentSession())
        {
            return;
        }

        auto &manager = engine.currentSession()->dialogueManager();
        if (!manager.isInDialogue())
        {
            return;
        }

        UIElement::update(engine, dt, parentOffset);

        _dialogueBlipCooldown -= dt.asSeconds();
        if (_textDisplayIndex < _text.size())
        {
            _textDisplay[_textDisplayIndex] = _text[_textDisplayIndex];
            _textDisplayIndex++;
            _textDisplay[_textDisplayIndex] = '\0';

            _contentText->text(_textDisplay.data());

            if (_dialogueBlipCooldown < 0.0f)
            {
                auto dialogueAudio = manager.dialogueAudio();
                if (dialogueAudio)
                {
                    auto blip = dialogueAudio->randomSoundBuffer();
                    if (blip)
                    {
                        _dialogueBlip->stop();
                        _dialogueBlip->setBuffer(*blip);
                        _dialogueBlip->play();

                        _dialogueBlipCooldown = blip->getDuration().asSeconds();
                    }
                }
            }
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

    void GameUIDialogue::onGameSessionStart()
    {
        auto &engine = _uiManager->engine();
        if (engine.currentSession())
        {
            auto &manager = engine.currentSession()->dialogueManager();
            _removeDialogueHandler = manager.onNextDialogue.createObserver([this, &manager]()
            {
                this->processText(manager);
            });
        }
    }
} // space