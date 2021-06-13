#include "npc_needs.hpp"

namespace space
{
    NpcNeeds::NpcNeeds(float hunger, float energy, float fun, float social) :
        _hunger(hunger), _energy(energy), _fun(fun), _social(social)
    {

    }

    void NpcNeeds::update(sf::Time dt)
    {
        auto seconds = dt.asSeconds();
        auto newHunger = _hunger - seconds / (60.0f * 1.0f);
        auto newEnergy = _energy - seconds / (60.0f * 10.0f);
        auto newFun = _fun - seconds / (60.0f * 3.0f);
        auto newSocial = _social - seconds / (60.0f * 13.0f);

        hunger(newHunger);
        energy(newEnergy);
        fun(newFun);
        social(newSocial);
    }
} // space