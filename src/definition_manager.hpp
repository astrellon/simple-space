#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>

#include "non_copyable.hpp"
#include "definitions/base_definition.hpp"

namespace space
{
    class Engine;

    class DefinitionManager : private NonCopyable
    {
        public:
            typedef std::map<DefinitionId, std::unique_ptr<BaseDefinition>> DefinitionMap;

            std::vector<const BaseDefinition *> load(const std::string &filename);
            void loadFolder(const std::string &folder);

            const DefinitionMap &definitions() const;

            void onPostLoad(Engine &engine);

            template<typename TDef>
            bool tryGet(const DefinitionId &id, const TDef **result) const
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

            template<typename TDef>
            bool tryGet(const DefinitionId &id, TDef **result)
            {
                auto find = _definitions.find(id);
                if (find == _definitions.end())
                {
                    return false;
                }

                auto casted = dynamic_cast<TDef *>(find->second.get());
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

