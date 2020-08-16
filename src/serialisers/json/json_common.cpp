#include "json_common.hpp"

namespace space
{
    template <>
    json toJsonArray(const std::vector<std::string> &input)
    {
        json result;
        for (auto &iter : input)
        {
            result.push_back(iter);
        }
        return result;
    }
} // namespace space
