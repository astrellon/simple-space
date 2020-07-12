#pragma once

#include <SFML/Graphics.hpp>

#include "game/space_object.hpp"

namespace space
{
    class Engine;

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

            void scale(float scale);
            float scale() const { return _scale;}

            void zoomScale(float zoomScale);
            float zoomScale() const { return _zoomScale;}

            void followingId(const ObjectId &id);
            const ObjectId &followingId() const { return _followingId; }

            void following(bool follow);
            bool isFollowing() const { return _following; }

            void followingRotationId(const ObjectId &id);
            const ObjectId &followingRotationId() const { return _followingRotationId; }

            void followingRotation(bool follow);
            bool isFollowingRotation() const { return _followingRotation; }

            float getRotation() const;

            const sf::View &view() const;

        private:
            // Fields
            Engine &_engine;
            sf::View _view;
            sf::Vector2f _size;
            float _scale;
            float _zoomScale;

            bool _following;
            bool _followingRotation;
            ObjectId _followingId;
            ObjectId _followingRotationId;

            // Methods
            void updateViewSize();
    };
} // space