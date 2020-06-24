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

            void following(const ObjectId &id);
            void following(bool follow);

            const sf::View &view() const;

        private:
            // Fields
            Engine &_engine;
            sf::View _view;
            sf::Vector2f _size;
            bool _following;
            float _scale;
            ObjectId _followingId;

            // Methods
    };
} // space