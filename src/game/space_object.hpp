#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "../non_copyable.hpp"
#include "../space_transform.hpp"
#include "interactions/interactable.hpp"
#include "../definitions/base_definition.hpp"

namespace space
{
    typedef std::string ObjectId;

    class GameSession;
    class StarSystem;
    class LoadingContext;

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

            void transform(const SpaceTransform &transform) { _transform = transform; }
            void transform(const SpaceTransform &&transform) { _transform = transform; }

            const sf::Transform &worldTransform() const { return _worldTransform; }

            void starSystem(StarSystem *starSystem) { _insideStarSystem = starSystem; }
            StarSystem *starSystem() const { return _insideStarSystem; }

            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform) = 0;
            virtual void draw(GameSession &session, sf::RenderTarget &target) = 0;

            virtual void onPostLoad(GameSession &session, LoadingContext &context) { }

            virtual bool doesMouseHover(GameSession &session, sf::Vector2f mousePosition) const { return false; }
            virtual bool isGenerated() const { return false; }

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