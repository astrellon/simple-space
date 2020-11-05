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
        sf::Time clonedTimeSinceStart;

        CloneContext(GameSession &session, bool isForLivePhoto, sf::IntRect photoArea, Area *insideArea, sf::Time clonedTimeSinceStart);
    };
} // namespace space
