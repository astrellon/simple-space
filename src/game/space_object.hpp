#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "../non_copyable.hpp"
#include "../space_transform.hpp"

namespace space
{
    typedef std::string ObjectId;

    class Engine;
    class StarSystem;

    class SpaceObject : private NonCopyable
    {
        public:
            // Fields
            const ObjectId id;

            // Constructor
            SpaceObject(const ObjectId &id) : id(id), _insideStarSystem(nullptr) { }
            virtual ~SpaceObject() { }

            // Methods
            const SpaceTransform &transform() const { return _transform; }
            SpaceTransform &transform() { return _transform; }

            const sf::Transform &worldTransform() const { return _worldTransform; }

            void starSystem(StarSystem *starSystem) { _insideStarSystem = starSystem; }
            StarSystem *starSystem() const { return _insideStarSystem; }

            virtual void update(Engine &engine, sf::Time dt, const sf::Transform &parentTransform) = 0;
            virtual void draw(Engine &engine, sf::RenderTarget &target) = 0;

        protected:
            // Fields
            SpaceTransform _transform;
            sf::Transform _worldTransform;
            StarSystem *_insideStarSystem;

            // Methods
            void updateWorldTransform(const sf::Transform &parentTransform)
            {
                _worldTransform = parentTransform * _transform.getTransform();
            }
    };
} // space