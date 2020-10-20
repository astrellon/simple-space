#pragma once

#include <vector>
#include <memory>

#include "space_object.hpp"

namespace space
{
    class RenderCamera;
    class GameSession;
    class Engine;

    class LivePhoto : public SpaceObject
    {
        public:
            // Fields

            // Constructor
            LivePhoto(const ObjectId &id);
            ~LivePhoto() { }

            // Methods
            virtual SpaceObject *clone(const ObjectId &newId, GameSession &session) { return cloneLivePhoto(newId, session); }
            LivePhoto *cloneLivePhoto(const ObjectId &newId, GameSession &session);

            void init(Engine &engine, sf::Vector2f size, float cameraScale);

            static const std::string SpaceObjectType() { return "live-photo"; }
            virtual std::string type() const { return SpaceObjectType(); }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, RenderCamera &target);

            void rootObject(SpaceObject *obj) { _rootObject = obj; }
            SpaceObject *rootObject() const { return _rootObject; }

        private:
            // Fields
            SpaceObject *_rootObject;
            std::unique_ptr<RenderCamera> _camera;
            sf::Sprite _cameraSprite;

            // Methods
    };
} // space