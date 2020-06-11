#pragma once

#include <memory>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include "serialisers/json/json.hpp"
#include "serialisers/json/json_serialisers.hpp"

#include "non_copyable.hpp"
#include "definitions/base_definition.hpp"
#include "definitions/ship_definition.hpp"

namespace space
{
    class DefinitionManager : private NonCopyable
    {
        public:
            typedef std::map<DefinitionId, std::unique_ptr<BaseDefinition>> DefinitionMap;

            template <typename TDef>
            const TDef *load(const std::string &filename)
            {
                auto find = _definitions.find(filename);
                if (find != _definitions.end())
                {
                    return nullptr;
                }

                std::ifstream file;
                file.open(filename);

                if (!file)
                {
                    std::cout << "Unable to find definition file " << filename << std::endl;
                    return nullptr;
                }

                nlohmann::json json;
                file >> json;

                auto temp = from_json_base_definition(json);
                auto result = dynamic_cast<TDef *>(temp.get());
                if (result == nullptr)
                {
                    return result;
                }

                _definitions.emplace(filename, std::move(temp));

                //auto *temp = json.get<TDef *>();
                //auto *temp = json.get<space::ShipDefinition *>();
                //_definitions.emplace(filename, std::unique_ptr<TDef>(temp));
                return result;
            }

            const DefinitionMap &definitions() const;

        private:

            DefinitionMap _definitions;
    };
}

