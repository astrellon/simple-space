#include "clone_context.hpp"

#include "space_object.hpp"
#include "area.hpp"

namespace space
{
    CloneContext::CloneContext(GameSession &session, bool isForLivePhoto, sf::IntRect photoArea, Area *insideArea) :
            session(session), isForLivePhoto(isForLivePhoto), photoArea(photoArea), insideArea(insideArea)
    {
        auto area = insideArea;
        while (area)
        {
            showingAreas.push_back(area);
            area = area->partOfObject()->insideArea();
        }
    }
} // space