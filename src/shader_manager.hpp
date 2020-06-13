#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <map>

#include "non_copyable.hpp"

namespace space
{
    class ShaderManager : private NonCopyable
    {
        public:
            typedef std::map<std::string, std::unique_ptr<sf::Shader>> ShaderMap;

            sf::Shader *load(const std::string &name, const std::string &vertShaderFilename, const std::string &fragShaderFilename);

            bool tryGet(const std::string &name, sf::Shader **result) const;

            const ShaderMap &shaders() const;

        private:

            ShaderMap _shaders;
    };
}

