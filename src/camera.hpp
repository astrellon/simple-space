#pragma once

#include <SFML/Graphics.hpp>

#include "types.hpp"

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
            const std::string debugName;

            // Constructor
            Camera(Engine &engine, std::string debugName);

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
            void setZoomScaleFromEngine();

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

            sf::FloatRect viewport() const;

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