#include "star_background_options.hpp"

#include "../engine.hpp"
#include "shader_definition.hpp"

namespace space
{
    StarBackgroundOptions::StarBackgroundOptions() :
        shaderName("EFFECT_STARS"),
        backgroundColour(0, 0, 0),
        numParticles(2048),
        area(1024),
        shader(nullptr),
        starColours(nullptr)
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
