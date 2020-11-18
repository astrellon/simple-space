#include "clone_context.hpp"

#include "space_object.hpp"
#include "area.hpp"

namespace space
{
    CloneContext::CloneContext(GameSession &session, sf::IntRect photoArea, Area *insideArea, sf::Time clonedTimeSinceStart) :
            session(session), photoArea(photoArea), insideArea(insideArea), clonedTimeSinceStart(clonedTimeSinceStart)
    {
        auto area = insideArea;
        while (area)
        {
            showingAreas.push_back(area);
            area = area->partOfObject()->insideArea();
        }
    }
} // space