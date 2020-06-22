#pragma once

#include <vector>

#include "../non_copyable.hpp"
#include "../definitions/star_system_definition.hpp"

namespace space
{
    class SpaceObject;

    class StarSystem
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

        private:
            // Fields
            std::vector<SpaceObject *> _objects;

            // Methods
    };
} // space