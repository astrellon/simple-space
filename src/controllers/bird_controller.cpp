#include "bird_controller.hpp"

#include "../utils.hpp"
#include "../game/character.hpp"

namespace space
{
    BirdController::BirdController(GameSession &session) : CharacterController(session), _timeToBlink(0.0f)
    {

    }

    void BirdController::update(sf::Time dt)
    {
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