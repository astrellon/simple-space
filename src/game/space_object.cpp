#include "space_object.hpp"

#include "../game_session.hpp"
#include "area.hpp"

namespace space
{
    SpaceObject *SpaceObject::rootObject()
    {
        if (_insideArea && _insideArea->partOfObject())
        {
            return _insideArea->partOfObject()->rootObject();
        }

        return this;
    }
} // namespace space
