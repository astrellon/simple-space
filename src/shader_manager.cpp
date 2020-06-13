#include "shader_manager.hpp"

#include <iostream>
#include <filesystem>

namespace space
{
    sf::Shader *ShaderManager::load(const std::string &name, const std::string &vertShaderFilename, const std::string &fragShaderFilename)
    {
        auto find = _shaders.find(name);
        if (find != _shaders.end())
        {
            std::cout << "Shader already loaded: " << name << std::endl;
            return find->second.get();
        }

        auto shader = std::make_unique<sf::Shader>();
        if (!shader->loadFromFile(vertShaderFilename, fragShaderFilename))
        {
            std::cout << "Unable to load shader: " << vertShaderFilename << ", " << fragShaderFilename << std::endl;
            return nullptr;
        }

        const auto result = shader.get();
        shader.swap(_shaders[name]);
        return result;
    }

    bool ShaderManager::tryGet(const std::string &name, sf::Shader **result) const
    {
        auto find = _shaders.find(name);
        if (find != _shaders.end())
        {
            *result = find->second.get();
            return true;
        }

        return false;
    }

    const ShaderManager::ShaderMap &ShaderManager::shaders() const
    {
        return _shaders;
    }
}