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
            std::map<Area *, std::unique_ptr<AreaInstances>> postLoadAreaInstances;
            std::map<ObjectId, ObjectId> livePhotos;

            // Constructor

            // Methods
            AreaInstances *getAreaInstance(Area *area)
            {
                auto find = postLoadAreaInstances.find(area);
                if (find == postLoadAreaInstances.end())
                {
                    auto instances = std::make_unique<AreaInstances>();
                    auto result = instances.get();
                    postLoadAreaInstances[area] = std::move(instances);
                    return result;
                }

                return find->second.get();
            }

            bool tryGetLivePhotoId(const ObjectId &id, ObjectId &livePhotoId)
            {
                auto find = livePhotos.find(id);
                if (find == livePhotos.end())
                {
                    return false;
                }

                *(&livePhotoId) = find->second;
                return true;
            }

        private:
            // Fields

            // Methods
    };
} // space