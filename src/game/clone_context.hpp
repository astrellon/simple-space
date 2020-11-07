#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

namespace space
{
    class GameSession;
    class Area;
    class LivePhoto;

    struct CloneContext
    {
        GameSession &session;
        LivePhoto *isForLivePhoto;
        sf::IntRect photoArea;
        Area *insideArea;
        std::vector<Area *> showingAreas;
        sf::Time clonedTimeSinceStart;

        CloneContext(GameSession &session, LivePhoto *isForLivePhoto, sf::IntRect photoArea, Area *insideArea, sf::Time clonedTimeSinceStart);
    };
} // namespace space
