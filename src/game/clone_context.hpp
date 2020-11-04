#pragma once

#include <SFML/Graphics.hpp>

namespace space
{
    class GameSession;

    struct CloneContext
    {
        GameSession &session;
        bool isForLivePhoto;
        sf::IntRect photoArea;

        CloneContext(GameSession &session, bool isForLivePhoto, sf::IntRect photoArea) :
            session(session), isForLivePhoto(isForLivePhoto), photoArea(photoArea)
        { }
    };
} // namespace space
