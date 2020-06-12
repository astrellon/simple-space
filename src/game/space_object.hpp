#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "../non_copyable.hpp"
#include "../space_transform.hpp"

namespace space
{
    typedef std::string ObjectId;

    class SpaceObject : private NonCopyable
    {
        public:
            // Fields
            const ObjectId id;

            // Constructor
            SpaceObject(const ObjectId &id) : id(id) { }

            // Methods
            const SpaceTransform &transform() const { return _transform; }
            SpaceTransform &transform() { return _transform; }

            virtual void update(sf::Time dt) = 0;
            virtual void draw(sf::RenderTarget &target, const sf::Transform &parentTransform) = 0;

        protected:
            // Fields
            //sf::Transform _transform;
            SpaceTransform _transform;

            // Methods
    };
} // space