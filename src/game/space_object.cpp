#include "space_object.hpp"

#include "../game_session.hpp"
#include "area.hpp"
#include "clone_context.hpp"
#include "../serialisers/loading_context.hpp"
#include "live_photo.hpp"

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

    void SpaceObject::populateCloneFromThis(SpaceObject *newClone, const CloneContext &context)
    {
        newClone->transform(_transform);
        newClone->partOfLivePhoto(context.isForLivePhoto);
    }

    void SpaceObject::onPostLoad(GameSession &session, LoadingContext &context)
    {
        ObjectId livePhotoId;
        if (context.tryGetLivePhotoId(id, livePhotoId))
        {
            session.tryGetSpaceObject<LivePhoto>(livePhotoId, &_partOfLivePhoto);
        }
    }

    void SpaceObject::loopOver(LoopObjectCallback callback)
    {
        callback(this);
    }
} // namespace space
