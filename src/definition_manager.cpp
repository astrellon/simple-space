#include "definition_manager.hpp"

#include <filesystem>

#include "utils.hpp"

namespace space
{
    std::vector<const BaseDefinition *> DefinitionManager::load(const std::string &filename)
    {
        std::vector<const BaseDefinition *> result;

        std::ifstream file;
        file.open(filename);

        if (!file)
        {
            std::cout << "Unable to find definition file " << filename << std::endl;
            return result;
        }

        nlohmann::json json;
        file >> json;

        if (!json.is_array())
        {
            json = json::array({json});
        }

        for (auto it : json)
        {
            auto temp = from_json_base_definition(it);
            if (temp == nullptr)
            {
                std::cout << "Unable to parse definition json" << std::endl;
                continue;
            }

            std::cout << "- Loaded: " << temp->id << std::endl;

            result.push_back(temp.get());
            temp.swap(_definitions[temp->id]);
        }

        return result;
    }

    void DefinitionManager::load_folder(const std::string &folder)
    {
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(folder))
        {
            if (dirEntry.is_directory())
            {
                continue;
            }

            const auto &filename = dirEntry.path().string();

            std::cout << "Loading definitions: " << filename << std::endl;

            load(filename);
        }
    }

    const DefinitionManager::DefinitionMap &DefinitionManager::definitions() const
    {
        return _definitions;
    }

    void DefinitionManager::on_post_load(Engine &engine)
    {
        for (auto &it : _definitions)
        {
            it.second->on_post_load(engine);
        }
    }
}

