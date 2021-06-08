#include "bird_controller.hpp"

#include "../utils.hpp"
#include "../game/character.hpp"

#include "./actions/npc_action_move.hpp"

namespace space
{
    BirdController::BirdController(GameSession &session) : NpcController(session), _timeToBlink(0.0f), _timeToNextAction(0.0f)
    {

    }

    void BirdController::update(sf::Time dt)
    {
        if (_character->insideArea() == nullptr)
        {
            return;
        }

        NpcController::update(dt);

        if (_highLevelActions.size() == 0)
        {
            if (_timeToNextAction <= 0.0f)
            {
                sf::Vector2f newPos;
                do
                {
                    auto pos = _character->transform().position;
                    pos.x = Utils::randf(pos.x - 50, pos.x + 50);
                    pos.y = Utils::randf(pos.y - 50, pos.y + 50);
                    newPos = pos;
                } while (newPos.x < 50 || newPos.y < 50);

                _highLevelActions.push(std::make_unique<NpcActionMove>(this, newPos));
                _timeToNextAction = Utils::randf(2, 4);
            }
            else
            {
                _timeToNextAction -= dt.asSeconds();
            }
        }

        if (_character->moveInput.x < 0)
        {
            _character->flipSprite(false);
        }
        else if (_character->moveInput.x > 0)
        {
            _character->flipSprite(true);
        }

        _timeToBlink -= dt.asSeconds();
        if (_timeToBlink < 0.0f)
        {
            _timeToBlink += Utils::randf(2.0f, 5.0f);
            auto &sprite = _character->sprite();
            sprite.sequence("blink", true);
            sprite.sequence("idle", false);
        }
    }
} // space