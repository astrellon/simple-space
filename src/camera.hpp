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
            void setSize(sf::Vector2f size);
            void setFollowing(const ObjectId &id);
            void setFollowing(bool follow);

            const sf::View &view() const;

        private:
            // Fields
            Engine &_engine;
            sf::View _view;
            bool _following;
            ObjectId _followingId;

            // Methods
    };
} // space