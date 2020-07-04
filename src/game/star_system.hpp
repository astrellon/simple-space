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
    class GameSession;

    class StarSystem : private NonCopyable
    {
        public:
            // Fields
            const StarSystemDefinition &definition;

            // Constructor
            StarSystem(GameSession &session, const StarSystemDefinition &definition);

            // Methods
            const std::vector<SpaceObject *> objects() const
            {
                return _objects;
            }

            void initFromDefinition();
            void addObject(SpaceObject *object);
            void removeObject(SpaceObject *object);

            void update(sf::Time dt);
            void draw(sf::RenderTarget &target);

        private:
            // Fields
            std::vector<SpaceObject *> _objects;
            GameSession &_session;

            // Methods
            void createCelestialBody(const CelestialBodyDefinition *bodyDefinition);

    };
} // space