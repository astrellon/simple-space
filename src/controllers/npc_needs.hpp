#pragma once

#include <SFML/System.hpp>

#include "../utils.hpp"

namespace space
{
    class NPCNeeds
    {
        public:
            // Fields

            // Constructor
            NPCNeeds(float hunger = 0, float energy = 0, float fun = 0, float social = 0);

            // Methods
            void update(sf::Time dt);

            float hunger() const { return _hunger; }
            void hunger(float hunger) { _hunger = Utils::clamp(hunger, -1, 1); }

            float energy() const { return _energy; }
            void energy(float energy) { _energy = Utils::clamp(energy, -1, 1); }

            float fun() const { return _fun; }
            void fun(float fun) { _fun = Utils::clamp(fun, -1, 1); }

            float social() const { return _social; }
            void social(float social) { _social = Utils::clamp(social, -1, 1); }

        private:
            // Fields
            float _hunger;
            float _energy;
            float _fun;
            float _social;

            // Methods
    };
} // space