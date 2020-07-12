#include "star_background_options.hpp"

#include "../engine.hpp"

namespace space
{
    StarBackgroundOptions::StarBackgroundOptions() :
        shaderName("stars"), numLayers(7), numParticles(200), backgroundColour(0, 0, 0), area(800)
    {

    }

    void StarBackgroundOptions::onPostLoad(Engine &engine)
    {
        engine.resourceManager().shader(shaderName, &shader);
    }
} // namespace space
