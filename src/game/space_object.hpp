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

            void starSystem(StarSystem *starSystem) { _insideStarSystem = starSystem; }
            StarSystem *starSystem() const { return _insideStarSystem; }

            virtual void update(Engine &engine, sf::Time dt) = 0;
            virtual void draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform) = 0;

        protected:
            // Fields
            SpaceTransform _transform;
            StarSystem *_insideStarSystem;

            // Methods
    };
} // space