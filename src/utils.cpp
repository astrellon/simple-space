#include "utils.hpp"

#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <math.h>

namespace space
{
    std::random_device Utils::_rd;
    std::mt19937 Utils::_rand(Utils::_rd());
    siv::PerlinNoise Utils::_perlinNoise(Utils::_rd());

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

    std::string Utils::filenameWithoutExt(const std::string &str)
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

    std::string Utils::getFilenameExt(const std::string &str)
    {
        auto dotIndex = str.find_last_of('.');
        if (dotIndex == std::string::npos)
        {
            dotIndex = str.length();
        }

        return str.substr(dotIndex + 1);
    }

    std::mt19937 Utils::randWithSeed(int seed)
    {
        std::random_device rd;
        std::mt19937 rand(rd());
        rand.seed(seed);

        return rand;
    }

    sf::Color Utils::hsv(float hue, float saturation, float value)
    {
        auto C = saturation * value;
        auto X = C * (1 - abs(fmod(hue / 60.0f, 2) - 1));
        float m = value - C;
        float Rs, Gs, Bs;

        if (hue >= 0 && hue < 60)
        {
            Rs = C;
            Gs = X;
            Bs = 0;
        }
        else if (hue >= 60 && hue < 120)
        {
            Rs = X;
            Gs = C;
            Bs = 0;
        }
        else if (hue >= 120 && hue < 180)
        {
            Rs = 0;
            Gs = C;
            Bs = X;
        }
        else if (hue >= 180 && hue < 240)
        {
            Rs = 0;
            Gs = X;
            Bs = C;
        }
        else if (hue >= 240 && hue < 300)
        {
            Rs = X;
            Gs = 0;
            Bs = C;
        }
        else
        {
            Rs = C;
            Gs = 0;
            Bs = X;
        }

        auto r = (Rs + m) * 255;
        auto g = (Gs + m) * 255;
        auto b = (Bs + m) * 255;

        return sf::Color(r, g, b);
    }
}