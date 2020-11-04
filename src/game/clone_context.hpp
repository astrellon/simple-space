#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

namespace space
{
    class GameSession;
    class Area;

    struct CloneContext
    {
        GameSession &session;
        bool isForLivePhoto;
        sf::IntRect photoArea;
        Area *insideArea;
        std::vector<Area *> showingAreas;

        CloneContext(GameSession &session, bool isForLivePhoto, sf::IntRect photoArea, Area *insideArea);
    };
} // namespace space
