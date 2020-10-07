#pragma once

#include <map>
#include <memory>

#include "../game/area.hpp"
#include "../game/area_instances.hpp"

#include "../non_copyable.hpp"

namespace space
{
    class LoadingContext : private NonCopyable
    {
        public:
            // Fields
            std::map<Area *, std::unique_ptr<AreaInstances>> postLoadWalkableAreaInstances;

            // Constructor

            // Methods
            AreaInstances *getWalkableInstances(Area *area)
            {
                auto find = postLoadWalkableAreaInstances.find(area);
                if (find == postLoadWalkableAreaInstances.end())
                {
                    auto instances = std::make_unique<AreaInstances>();
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