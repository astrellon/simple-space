#include "shader_definition.hpp"

#include "../engine.hpp"

namespace space
{
    void ShaderDefinition::onPostLoad(Engine &engine)
    {
        const std::string *fragmentText;
        const std::string *vertexText;

        if (!engine.resourceManager().text(fragementPath, &fragmentText))
        {
            std::cout << "Unable to find fragment shader: " << fragementPath << " for " << id << std::endl;
            return;
        }
        if (!engine.resourceManager().text(vertexPath, &vertexText))
        {
            std::cout << "Unable to find vertex shader: " << fragementPath << " for " << id << std::endl;
            return;
        }

        if (!shader.loadFromMemory(*vertexText, *fragmentText))
        {
            std::cout << "Failed to load shader: " << id << std::endl;
        }
    }
} // space