#include "space_object_type.hpp"

namespace space
{
    const std::string toString(SpaceObjectType2 input)
    {
        switch (input)
        {
            default:
            case SpaceObjectType2::Unknown: return "unknown";
            case SpaceObjectType2::Character: return "character";
            case SpaceObjectType2::LivePhoto: return "live-photo";
            case SpaceObjectType2::LivePhotoTarget: return "live-photo-target";
            case SpaceObjectType2::ParticlesSimple: return "particles";
            case SpaceObjectType2::PlacedItem: return "placed-item";
            case SpaceObjectType2::Planet: return "planet";
            case SpaceObjectType2::PlanetSurface: return "planet-surface";
            case SpaceObjectType2::Ship: return "ship";
            case SpaceObjectType2::StarSystem: return "star-system";
            case SpaceObjectType2::SpacePortal: return "space-portal";
            case SpaceObjectType2::TeleportClone: return "teleport-clone";
            case SpaceObjectType2::GrassEffect: return "grass-effect";
        }
    }

    SpaceObjectType2 fromString(const std::string &input)
    {
        if (input == "character") return SpaceObjectType2::Character;
        if (input == "live-photo") return SpaceObjectType2::LivePhoto;
        if (input == "live-photo-target") return SpaceObjectType2::LivePhotoTarget;
        if (input == "particles") return SpaceObjectType2::ParticlesSimple;
        if (input == "placed-item") return SpaceObjectType2::PlacedItem;
        if (input == "planet") return SpaceObjectType2::Planet;
        if (input == "planet-surface") return SpaceObjectType2::PlanetSurface;
        if (input == "ship") return SpaceObjectType2::Ship;
        if (input == "star-system") return SpaceObjectType2::StarSystem;
        if (input == "space-portal") return SpaceObjectType2::SpacePortal;
        if (input == "teleport-clone") return SpaceObjectType2::TeleportClone;
        if (input == "grass-effect") return SpaceObjectType2::GrassEffect;

        return SpaceObjectType2::Unknown;
    }
} // space