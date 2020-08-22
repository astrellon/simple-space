#include "space_object.hpp"

#include "../game_session.hpp"

namespace space
{
    void SpaceObject::onPostLoad(GameSession &session)
    {
        // if (_insideStarSystemId.size() > 0)
        //     session.tryGetStarSystem(_insideStarSystemId, &_insideStarSystem);
        // else
        //     std::cout << "Unable to find star system '" << _insideStarSystemId << "' for space obj: " << id << std::endl;
    }
} // namespace space
