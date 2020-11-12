#pragma once

#include <string>
#include <functional>

namespace space
{
    class SpaceObject;

    typedef std::string ObjectId;
    typedef std::function<void(SpaceObject *)> LoopObjectCallback;

} // namespace space
