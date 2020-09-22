#pragma once

#include <vector>

#include "space_object.hpp"
#include "items/item.hpp"
#include "../non_copyable.hpp"

namespace space
{
    class WalkableArea;
    class GameSession;

    class WalkableAreaInstances : private NonCopyable
    {
        private:
            enum class PostLoadType
            {
                Character, GrassEffect, Item
            };

            struct PostLoadObject
            {
                ObjectId id;
                ItemId itemId;
                PostLoadType type;
                sf::Vector2f position;

                PostLoadObject(const ObjectId &id, PostLoadType type) : id(id), type(type) { }
                PostLoadObject(ItemId itemId, sf::Vector2f position) : itemId(itemId), type(PostLoadType::Item), position(position) { }
            };

        public:
            // Fields

            // Constructor

            // Methods
            void addPostLoadCharacter(const ObjectId &id) { _onPostLoadObjects.emplace_back(id, PostLoadType::Character); }
            void addPostLoadGrassEffect(const ObjectId &id) { _onPostLoadObjects.emplace_back(id, PostLoadType::GrassEffect); }
            void addPostLoadPlaceable(ItemId id, sf::Vector2f position) { _onPostLoadObjects.emplace_back(id, position); }

            void applyToWalkableArea(WalkableArea &walkableArea, GameSession &session);

        private:
            // Fields
            std::vector<PostLoadObject> _onPostLoadObjects;

            // Methods
    };
} // space