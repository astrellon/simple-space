#pragma once

#include "../game/space_object.hpp"

namespace space
{
    class EditorCameraTarget : public SpaceObject
    {
        public:
            // Fields
            static const SpaceObjectType TypeValue;

            // Constructor
            EditorCameraTarget(const ObjectId &id);

            // Methods
            virtual SpaceObjectType type() const { return TypeValue; }

            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) { return cloneEditorCameraTarget(newId, context); }
            EditorCameraTarget *cloneEditorCameraTarget(const ObjectId &newId, const CloneContext &context);

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform) { }
            virtual void draw(GameSession &session, RenderCamera &target) { }

            virtual bool isGenerated() const { return true; }
    };
} // space