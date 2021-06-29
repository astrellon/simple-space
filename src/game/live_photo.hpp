#pragma once

#include <vector>
#include <memory>

#include "space_object.hpp"
#include "../render_camera.hpp"

namespace space
{
    class GameSession;
    class Engine;

    class LivePhotoTarget : public SpaceObject
    {
        public:
            // Fields
            const static SpaceObjectType TypeValue;

            // Constructor
            LivePhotoTarget(const ObjectId &id);
            ~LivePhotoTarget() { }

            // Methods
            virtual SpaceObjectType type() const { return TypeValue; }

            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) { return cloneLivePhotoTarget(newId, context); }
            LivePhotoTarget *cloneLivePhotoTarget(const ObjectId &newId, const CloneContext &context);

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform) { }
            virtual void draw(GameSession &session, RenderCamera &target) { }
    };

    class LivePhoto : public SpaceObject
    {
        public:
            // Fields
            static SpaceObjectType TypeValue;

            const sf::Vector2u photoSize;

            // Constructor
            LivePhoto(const ObjectId &id, const sf::Vector2u renderSize);
            ~LivePhoto() { }

            // Methods
            virtual SpaceObjectType type() const { return TypeValue; }

            virtual SpaceObject *clone(const ObjectId &newId, const CloneContext &context) { return cloneLivePhoto(newId, context); }
            LivePhoto *cloneLivePhoto(const ObjectId &newId, const CloneContext &context);

            void init(Engine &engine);

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);
            virtual void onPostLoad(GameSession &session, LoadingContext &context);

            void updateInternalTarget(GameSession &session, sf::Time dt);
            void drawToInternalTexture(GameSession &session);

            sf::RenderTexture &texture() const;

            void targetObject(SpaceObject *obj);
            SpaceObject *targetObject() const { return _targetObject; }

            std::vector<SpaceObject *> getObjectsWithCompendium() const;

        private:
            // Fields
            SpaceObject *_targetObject;
            std::unique_ptr<RenderCamera> _camera;
            sf::Sprite _cameraSprite;
            int _lastFrameUpdate;
            int _lastFrameDraw;

            // Methods
    };
} // space