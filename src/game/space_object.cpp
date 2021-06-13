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
    }

    void SpaceObject::onPostLoad(GameSession &session, LoadingContext &context)
    {
        ObjectId livePhotoId;
        if (context.tryGetLivePhotoId(id, livePhotoId))
        {
            session.tryGetSpaceObject<LivePhoto>(livePhotoId, &_partOfLivePhoto);
        }
    }

    bool SpaceObject::loopOver(LoopObjectCallback callback)
    {
        return callback(this);
    }

    void SpaceObject::createInteractable()
    {
        if (!_interactable.get())
        {
            _interactable = std::make_unique<Interactable>(this);
        }
    }
} // namespace space
