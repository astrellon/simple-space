#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "../non_copyable.hpp"
#include "../space_transform.hpp"
#include "interactions/interactable.hpp"

namespace space
{
    typedef std::string ObjectId;

    class GameSession;
    class StarSystem;

    class SpaceObject : private NonCopyable
    {
        public:
            // Fields
            const ObjectId id;

            // Constructor
            SpaceObject(const ObjectId &id) : id(id), _insideStarSystem(nullptr), _interactable(this) { }
            virtual ~SpaceObject() { }

            // Methods
            virtual std::string type() const = 0;

            const SpaceTransform &transform() const { return _transform; }
            SpaceTransform &transform() { return _transform; }

            const sf::Transform &worldTransform() const { return _worldTransform; }

            void starSystem(StarSystem *starSystem) { _insideStarSystem = starSystem; }
            StarSystem *starSystem() const { return _insideStarSystem; }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform) = 0;
            virtual void draw(GameSession &session, sf::RenderTarget &target) = 0;

            Interactable &interactable() { return _interactable; }

        protected:
            // Fields
            SpaceTransform _transform;
            sf::Transform _worldTransform;
            StarSystem *_insideStarSystem;
            Interactable _interactable;

            // Methods
            void updateWorldTransform(const sf::Transform &parentTransform)
            {
                _worldTransform = parentTransform * _transform.getTransform();
            }
    };
} // space