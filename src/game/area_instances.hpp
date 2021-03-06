#pragma once

#include <vector>

#include "items/item.hpp"

#include "../types.hpp"
#include "../non_copyable.hpp"
#include "../definitions/grass_effect_definition.hpp"

namespace space
{
    class Area;
    class GameSession;

    class AreaInstances : private NonCopyable
    {
        public:
            // Fields

            // Constructor

            // Methods
            void addPostLoadObject(const ObjectId &id)
            {
                _onPostLoadObjects.emplace_back(id);
            }

            void applyToArea(Area &area, GameSession &session) const;

        private:
            // Fields
            std::vector<ObjectId> _onPostLoadObjects;

            // Methods
    };
} // space