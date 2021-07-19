#include "area_instances.hpp"

#include <iostream>

#include "character.hpp"
#include "area.hpp"
#include "items/placeable_item.hpp"

#include "../effects/grass_effect.hpp"
#include "../game_session.hpp"
#include "../engine.hpp"
#include "../definition_manager.hpp"

namespace space
{
    void AreaInstances::applyToArea(Area &area, GameSession &session) const
    {
        for (auto postLoad : _onPostLoadObjects)
        {
            SpaceObject *character;
            if (session.tryGetSpaceObject(postLoad, character))
            {
                area.addObject(character);
            }
            else
            {
                std::cout << "Unable to find space object '" << postLoad << "' for area" << std::endl;
            }
        }
    }
} // space