#include "walkable_area_instances.hpp"

#include <iostream>

#include "character.hpp"
#include "walkable_area.hpp"
#include "items/placeable_item.hpp"

#include "../effects/grass_effect.hpp"
#include "../game_session.hpp"

namespace space
{
    void WalkableAreaInstances::applyToWalkableArea(WalkableArea &walkableArea, GameSession &session)
    {
        for (auto postLoad : _onPostLoadObjects)
        {
            if (postLoad.type == PostLoadType::Character)
            {
                Character *character;
                if (session.tryGetSpaceObject<Character>(postLoad.id, &character))
                {
                    walkableArea.addCharacter(character);
                }
                else
                {
                    std::cout << "Unable to find character '" << postLoad.id << "' for walkable area" << std::endl;
                }
            }
            else if (postLoad.type == PostLoadType::GrassEffect)
            {
                GrassEffect *grassEffect;
                if (session.tryGetSpaceObject<GrassEffect>(postLoad.id, &grassEffect))
                {
                    walkableArea.addGrassEffect(grassEffect);
                }
                else
                {
                    std::cout << "Unable to find grass effect '" << postLoad.id << "' for walkable area" << std::endl;
                }
            }
            else if (postLoad.type == PostLoadType::Item)
            {
                PlaceableItem *item;
                if (!session.tryGetItem<PlaceableItem>(postLoad.itemId, &item))
                {
                    std::cout << "Unable to find placeable item for placed item: " << postLoad.id << std::endl;
                    continue;
                }

                walkableArea.addPlaceable(item, postLoad.position);
            }
        }
    }
} // space