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

    bool Utils::tryGetIntersection(const sf::Vector2f &origin, const sf::Vector2f &direction, const sf::Vector2f &point1, const sf::Vector2f &point2, sf::Vector2f *result)
    {
        auto v1 = origin - point1;
        auto v2 = point2 - point1;
        sf::Vector2 v3(-direction.y, direction.x);

        auto dot = v2.dot(v3);
        if (std::abs(dot) < 0.00001f)
        {
            return false;
        }

        auto t1 = v2.cross(v1) / dot;
        auto t2 = (v1.dot(v3)) / dot;

        if (t1 >= 0.0f & (t2 >= 0.0f && t2 <= 1.0f))
        {
            *result = origin + direction * t1;
            return true;
        }

        return false;
    }

    bool Utils::checkIfLinesIntersect(const sf::Vector2f &start1, const sf::Vector2f &end1, const sf::Vector2f &start2, const sf::Vector2f &end2)
    {
        return counterClockWise(start1, start2, end2) != counterClockWise(end1, start2, end2) &&
            counterClockWise(start1, end1, start2) != counterClockWise(start1, end1, end2);
    }

    float Utils::counterClockWise(const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3)
    {
        return (p3.y - p1.y) * (p2.x - p1.x) > (p2.y - p1.y) * (p3.x - p1.x);
    }
}