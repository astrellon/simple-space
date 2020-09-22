#pragma once

#include <vector>

#include "space_object.hpp"
#include "items/item.hpp"
#include "../non_copyable.hpp"
#include "../definitions/grass_effect_definition.hpp"

namespace space
{
    class WalkableArea;
    class GameSession;

    class WalkableAreaInstances : private NonCopyable
    {
        private:
            struct PostLoadItem
            {
                ItemId itemId;
                sf::Vector2f position;

                PostLoadItem(ItemId itemId, sf::Vector2f position) : itemId(itemId), position(position) { }
            };

            struct PostLoadGrass
            {
                DefinitionId defId;
                sf::Vector2f position;

                PostLoadGrass(const DefinitionId &defId, sf::Vector2f position) : defId(defId), position(position) { }
            };

        public:
            // Fields

            // Constructor

            // Methods
            void addPostLoadCharacter(const ObjectId &id)
            {
                _onPostLoadObjects.emplace_back(id);
            }
            void addPostLoadGrassEffect(const DefinitionId &id, sf::Vector2f position)
            {
                _onPostLoadGrass.emplace_back(id, position);
            }
            void addPostLoadPlaceable(ItemId id, sf::Vector2f position)
            {
                _onPostLoadItems.emplace_back(id, position);
            }

            void applyToWalkableArea(WalkableArea &walkableArea, GameSession &session) const;

        private:
            // Fields
            std::vector<PostLoadItem> _onPostLoadItems;
            std::vector<PostLoadGrass> _onPostLoadGrass;
            std::vector<ObjectId> _onPostLoadObjects;

            // Methods
    };
} // space