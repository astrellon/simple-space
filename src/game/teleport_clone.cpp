#include "teleport_clone.hpp"

#include "../game_session.hpp"
#include "../engine.hpp"
#include "../debug/draw_debug.hpp"

#include "character.hpp"

namespace space
{
    TeleportClone::TeleportClone(const ObjectId &id, const Character &clone, const SpaceTransform &preTransform) :
        Character(id, clone.definition), _cloneId(clone.id), _preTeleport(preTransform), _postTeleport(clone.transform()), _cloneFound(nullptr)
    {

    }

    void TeleportClone::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        if (!tryGetClone(session, &_cloneFound))
        {
            return;
        }

        auto diffPos = _cloneFound->transform().position - _postTeleport.position;
        auto diffRotation = _cloneFound->transform().rotation - _postTeleport.rotation;
        _transform.position = _preTeleport.position + diffPos;
        _transform.rotation = _preTeleport.rotation + diffRotation;

        updateWorldTransform(parentTransform);

        if (_sprite.currentAnimation() != _cloneFound->sprite().currentAnimation())
        {
            _sprite.sequence(_cloneFound->sprite().currentAnimation(), true);
        }
        _sprite.animationTime = _cloneFound->sprite().animationTime;
        _sprite.update(sf::Time::Zero);
    }

    void TeleportClone::draw(GameSession &session, sf::RenderTarget &target)
    {
        if (!_cloneFound)
        {
            return;
        }

        target.draw(_sprite, _worldTransform);
        DrawDebug::glDraw++;
    }

    bool TeleportClone::tryGetClone(GameSession &session, Character **result)
    {
        SpaceObject *cloneObject;
        if (!session.tryGetSpaceObject(_cloneId, &cloneObject))
        {
            *result = nullptr;
            return false;

        }

        Character *clone = dynamic_cast<Character *>(cloneObject);
        if (clone == nullptr)
        {
            *result = nullptr;
            return false;
        }

        *result = clone;
        return true;
    }
} // namespace space
