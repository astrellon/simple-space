#include "editor_camera_target.hpp"

#include "../game_session.hpp"

namespace space
{
    const SpaceObjectType EditorCameraTarget::TypeValue = SpaceObjectType::EditorCameraTarget;

    EditorCameraTarget::EditorCameraTarget(const ObjectId &id) : SpaceObject(id)
    {

    }

    EditorCameraTarget *EditorCameraTarget::cloneEditorCameraTarget(const ObjectId &newId, const CloneContext &context)
    {
        auto result = context.session.createObject<EditorCameraTarget>(newId);
        populateCloneFromThis(result, context);
        return result;
    }

    void EditorCameraTarget::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);
    }
} // space