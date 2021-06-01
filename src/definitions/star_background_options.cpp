#include "star_background_options.hpp"

#include "../engine.hpp"
#include "shader_definition.hpp"

namespace space
{
    StarBackgroundOptions::StarBackgroundOptions() :
        shaderName("EFFECT_STARS"), numLayers(7), numParticles(1000), backgroundColour(0, 0, 0), area(800), shader(nullptr), starColours(nullptr)
    {

    }

    void StarBackgroundOptions::onPostLoad(Engine &engine)
    {
        ShaderDefinition *shaderDef;
        if (!engine.definitionManager().tryGet(shaderName, &shaderDef))
        {
            std::cout << "Unable to find star background shader" << std::endl;
        }
        else
        {
            shader = &shaderDef->shader;
        }

        engine.resourceManager().image(starColoursPath, &starColours);
    }
} // namespace space
