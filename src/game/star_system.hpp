#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

#include "../non_copyable.hpp"
#include "../definitions/star_system_definition.hpp"

namespace space
{
    typedef std::string StarSystemId;
    class Engine;
    class SpaceObject;

    class StarSystem : private NonCopyable
    {
        public:
            // Fields
            const StarSystemDefinition &definition;

            // Constructor
            StarSystem(const StarSystemDefinition &definition);

            // Methods
            const std::vector<SpaceObject *> objects() const
            {
                return _objects;
            }

            void addObject(SpaceObject *object);
            void removeObject(SpaceObject *object);

            void update(Engine &engine, sf::Time dt);
            void draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform);

        private:
            // Fields
            std::vector<SpaceObject *> _objects;

            // Methods
    };
} // space