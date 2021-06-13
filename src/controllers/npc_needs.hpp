#pragma once

#include <SFML/System.hpp>

#include "../utils.hpp"

namespace space
{
    class NpcNeeds
    {
        public:
            // Fields

            // Constructor
            NpcNeeds(float hunger = 0, float energy = 0, float fun = 0, float social = 0);

            // Methods
            void update(sf::Time dt);

            float hunger() const { return _hunger; }
            void hunger(float hunger) { _hunger = Utils::clamp(hunger, -1, 1); }
            void changeHunger(float delta) { hunger(_hunger + delta); }

            float energy() const { return _energy; }
            void energy(float energy) { _energy = Utils::clamp(energy, -1, 1); }
            void changeEnergy(float delta) { energy(_energy + delta); }

            float fun() const { return _fun; }
            void fun(float fun) { _fun = Utils::clamp(fun, -1, 1); }
            void changeFun(float delta) { fun(_fun + delta); }

            float social() const { return _social; }
            void social(float social) { _social = Utils::clamp(social, -1, 1); }
            void changeSocial(float delta) { social(_social + delta); }

        private:
            // Fields
            float _hunger;
            float _energy;
            float _fun;
            float _social;

            // Methods
    };
} // space