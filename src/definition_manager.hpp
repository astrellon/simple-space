#pragma once

#include <memory>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>

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

            std::vector<const BaseDefinition *> load(const std::string &filename);
            void load_folder(const std::string &folder);

            const DefinitionMap &definitions() const;

            template<typename TDef>
            bool try_get(const std::string &id, const TDef **result) const
            {
                auto find = _definitions.find(id);
                if (find == _definitions.end())
                {
                    return false;
                }

                auto casted = dynamic_cast<const TDef *>(find->second.get());
                if (casted == nullptr)
                {
                    return false;
                }

                *result = casted;
                return true;
            }

        private:

            DefinitionMap _definitions;
    };
}

