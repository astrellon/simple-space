#include "utils.hpp"

#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>

namespace space
{
    std::random_device Utils::_rd;
    std::mt19937 Utils::_rand(Utils::_rd());

    std::vector<std::string> Utils::split(const std::string &input, char delimiter, bool trimTokens)
    {
        std::vector<std::string> result;

        std::stringstream sstream(input);
        std::string token;
        while (std::getline(sstream, token, ','))
        {
            if (trimTokens)
            {
                trim(token);
            }
            result.push_back(token);
        }

        return result;
    }

    std::string &Utils::ltrim(std::string & str)
    {
        auto it2 = std::find_if(str.begin(), str.end(), [](char ch) { return !std::isspace<char>(ch, std::locale::classic()); });
        str.erase(str.begin(), it2);
        return str;
    }

    std::string &Utils::rtrim(std::string & str)
    {
        auto it1 = std::find_if(str.rbegin(), str.rend(), [](char ch) { return !std::isspace<char>(ch, std::locale::classic()); });
        str.erase(it1.base(), str.end());
        return str;
    }

    std::string &Utils::trim(std::string & str)
    {
        return ltrim(rtrim(str));
    }

    std::string Utils::filename_without_ext(const std::string &str)
    {
        int sepIndex = str.find_last_of('/');
        if (sepIndex == std::string::npos)
        {
            sepIndex = str.find_last_of('\\');
            if (sepIndex == std::string::npos)
            {
                sepIndex = -1;
            }
        }

        auto dotIndex = str.find_last_of('.');
        if (dotIndex == std::string::npos)
        {
            dotIndex = str.length();
        }

        return str.substr(sepIndex + 1, dotIndex - sepIndex - 1);
    }

    void Utils::readCSVLines(const std::string &filename, Utils::LineReader lineReader)
    {
        std::ifstream data(filename);

        std::string line;
        while (std::getline(data, line))
        {
            if (line.size() == 0 || line[0] == '#')
            {
                continue;
            }

            if (!lineReader(line))
            {
                break;
            }
        }
    }
}