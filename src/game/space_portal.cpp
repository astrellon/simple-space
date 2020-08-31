#include "space_portal.hpp"

#include "../debug/draw_debug.hpp"

namespace space
{
    SpacePortal::SpacePortal(const ObjectId &id, const SpacePortalDefinition &definition) :
        SpaceObject(id), definition(definition), _sprite(*definition.texture)
    {
        _sprite.sequence("active", true);
    }

    void SpacePortal::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);

        _sprite.update(dt);
    }

    void SpacePortal::draw(GameSession &session, sf::RenderTarget &target)
    {
        target.draw(_sprite, _worldTransform);
        DrawDebug::glDraw++;
    }

} // space