#include "json_serialisers_game.hpp"

#include "../../game_session.hpp"

namespace space
{
    json toJson(const GameSession &input)
    {
        auto result = json {
            //{"spaceObjects", input.spaceObjects}
        };

        return result;
    }

    std::unique_ptr<GameSession> fromJsonGameSession(Engine &engine, const json &j)
    {
        return std::make_unique<GameSession>(engine);
    }

} // namespace space
