#pragma once

#include <string>

namespace space
{
    enum class SpaceObjectType2
    {
        Unknown,
        Character,
        Ship,
        SpacePortal,
        LivePhoto,
        LivePhotoTarget,
        StarSystem,
        Planet,
        PlanetSurface,
        ParticlesSimple,
        PlacedItem,
        TeleportClone,
        GrassEffect
    };

    const std::string toString(SpaceObjectType2 input);
    SpaceObjectType2 fromStringSpaceObjectType(const std::string &input);
} // namespace space