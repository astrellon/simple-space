#pragma once

#include <string>

namespace space
{
    enum class SpaceObjectType
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
        GrassEffect,
        EditorCameraTarget
    };

    const std::string toString(SpaceObjectType input);
    SpaceObjectType fromStringSpaceObjectType(const std::string &input);
} // namespace space