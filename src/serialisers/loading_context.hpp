#pragma once

#include <map>
#include <memory>

#include "../game/walkable_area.hpp"
#include "../game/walkable_area_instances.hpp"

#include "../non_copyable.hpp"

namespace space
{
    class LoadingContext : private NonCopyable
    {
        public:
            // Fields
            std::map<WalkableArea *, std::unique_ptr<WalkableAreaInstances>> postLoadWalkableAreaInstances;

            // Constructor

            // Methods
            WalkableAreaInstances *getWalkableInstances(WalkableArea *area)
            {
                auto find = postLoadWalkableAreaInstances.find(area);
                if (find == postLoadWalkableAreaInstances.end())
                {
                    auto instances = std::make_unique<WalkableAreaInstances>();
                    auto result = instances.get();
                    postLoadWalkableAreaInstances[area] = std::move(instances);
                    return result;
                }

                return find->second.get();
            }

        private:
            // Fields

            // Methods
    };
} // space