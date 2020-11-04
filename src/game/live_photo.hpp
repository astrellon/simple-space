#pragma once

#include <vector>
#include <memory>

#include "space_object.hpp"

namespace space
{
    class RenderCamera;
    class GameSession;
    class Engine;

    class LivePhotoTarget : public SpaceObject
    {
        public:
            // Fields

            // Constructor
            LivePhotoTarget(const ObjectId &id);
            ~LivePhotoTarget() { }

            // Methods
            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) { return cloneLivePhotoTarget(newId, context); }
            LivePhotoTarget *cloneLivePhotoTarget(const ObjectId &newId, const CloneContext &context);

            static const std::string SpaceObjectType() { return "live-photo-target"; }
            virtual std::string type() const { return SpaceObjectType(); }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform) { }
            virtual void draw(GameSession &session, RenderCamera &target) { }
    };

    class LivePhoto : public SpaceObject
    {
        public:
            // Fields

            // Constructor
            LivePhoto(const ObjectId &id);
            ~LivePhoto() { }

            // Methods
            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) { return cloneLivePhoto(newId, context); }
            LivePhoto *cloneLivePhoto(const ObjectId &newId, const CloneContext &context);

            void init(Engine &engine, sf::Vector2f size, float cameraScale);

            static const std::string SpaceObjectType() { return "live-photo"; }
            virtual std::string type() const { return SpaceObjectType(); }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);

            void updateInternalTarget(GameSession &session, sf::Time dt);
            void drawToInternalTexture(GameSession &session);

            sf::RenderTexture &texture() const;

            void targetObject(SpaceObject *obj) { _targetObject = obj; }
            SpaceObject *targetObject() const { return _targetObject; }

        private:
            // Fields
            SpaceObject *_targetObject;
            std::unique_ptr<RenderCamera> _camera;
            sf::Sprite _cameraSprite;

            // Methods
    };
} // space