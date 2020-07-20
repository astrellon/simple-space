#pragma once

#include <SFML/Graphics.hpp>

#include "game/space_object.hpp"

namespace space
{
    class Engine;

    class CameraProps
    {
        public:
            // Fields
            float scale;

            bool following;
            bool followingRotation;
            ObjectId followingId;
            ObjectId followingRotationId;

            // Constructor
            CameraProps();
    };

    class Camera
    {
        public:
            // Fields

            // Constructor
            Camera(Engine &engine);

            // Methods
            void update(sf::Time dt);

            void size(sf::Vector2f size);
            sf::Vector2f size() const { return _size; }

            void center(sf::Vector2f center);
            sf::Vector2f center() const { return _view.getCenter(); }

            void rotation(float rotation);
            float rotation() const { return _view.getRotation(); }

            void scale(float scale);
            float scale() const { return _props.scale; }

            void zoomScale(float zoomScale);
            float zoomScale() const { return _zoomScale; }

            void followingId(const ObjectId &id);
            const ObjectId &followingId() const { return _props.followingId; }

            void following(bool follow);
            bool isFollowing() const { return _props.following; }

            void followingRotationId(const ObjectId &id);
            const ObjectId &followingRotationId() const { return _props.followingRotationId; }

            void followingRotation(bool follow);
            bool isFollowingRotation() const { return _props.followingRotation; }

            const CameraProps &cameraProps() const { return _props; }
            void cameraProps(const CameraProps &props);

            float getRotation() const;

            const sf::View &view() const;

        private:
            // Fields
            Engine &_engine;
            sf::View _view;
            sf::Vector2f _size;
            float _zoomScale;

            CameraProps _props;

            // Methods
            void updateViewSize();
    };
} // space