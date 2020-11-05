#include "cursor.hpp"

#include "../engine.hpp"

namespace space
{
    void Cursor::onPostLoad(Engine &engine)
    {
        engine.resourceManager().texture(texturePath, &texture);
    }
} // space