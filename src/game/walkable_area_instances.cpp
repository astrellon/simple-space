#include "walkable_area_instances.hpp"

#include <iostream>

#include "character.hpp"
#include "walkable_area.hpp"
#include "items/placeable_item.hpp"

#include "../effects/grass_effect.hpp"
#include "../game_session.hpp"
#include "../engine.hpp"
#include "../definition_manager.hpp"

namespace space
{
    void WalkableAreaInstances::applyToWalkableArea(WalkableArea &walkableArea, GameSession &session) const
    {
        for (auto postLoad : _onPostLoadObjects)
        {
            Character *character;
            if (session.tryGetSpaceObject<Character>(postLoad, &character))
            {
                walkableArea.addCharacter(character);
            }
            else
            {
                std::cout << "Unable to find character '" << postLoad << "' for walkable area" << std::endl;
            }
        }

        for (auto grassEffectEntry : _onPostLoadGrass)
        {
            GrassEffectDefinition *definition;
            if (session.engine().definitionManager().tryGet(grassEffectEntry.defId, &definition))
            {
                auto grassEffect = session.createObject<GrassEffect>(session.nextObjectId(), *definition);
                grassEffect->transform().position = grassEffectEntry.position;
                walkableArea.addGrassEffect(grassEffect);
            }
            else
            {
                std::cout << "Unable to find grass effect '" << grassEffectEntry.defId << "' for walkable area" << std::endl;
            }
        }

        for (auto placedItemEntry : _onPostLoadItems)
        {
            PlaceableItem *item;
            if (!session.tryGetItem<PlaceableItem>(placedItemEntry.itemId, &item))
            {
                std::cout << "Unable to find placeable item for placed item: " << placedItemEntry.itemId << std::endl;
                continue;
            }

            walkableArea.addPlaceable(item, placedItemEntry.position);
        }
    }
} // space