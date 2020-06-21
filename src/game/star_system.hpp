#pragma once

#include <vector>

#include "../non_copyable.hpp"

namespace space
{
    class StarSystem
    {
        public:
            // Fields

            // Constructor
            StarSystem();

            // Methods
            const std::vector<const Space Object *> objects() const
            {
                return _objects;
            }

        private:
            // Fields
            std::vector<SpaceObject *> _objects;

            // Methods
    };
} // space