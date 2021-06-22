#include "space_object_type.hpp"

namespace space
{
    const std::string toString(SpaceObjectType input)
    {
        switch (input)
        {
            default:
            case SpaceObjectType::Unknown: return "unknown";
            case SpaceObjectType::Character: return "character";
            case SpaceObjectType::LivePhoto: return "live-photo";
            case SpaceObjectType::LivePhotoTarget: return "live-photo-target";
            case SpaceObjectType::ParticlesSimple: return "particles";
            case SpaceObjectType::PlacedItem: return "placed-item";
            case SpaceObjectType::Planet: return "planet";
            case SpaceObjectType::PlanetSurface: return "planet-surface";
            case SpaceObjectType::Ship: return "ship";
            case SpaceObjectType::StarSystem: return "star-system";
            case SpaceObjectType::SpacePortal: return "space-portal";
            case SpaceObjectType::TeleportClone: return "teleport-clone";
            case SpaceObjectType::GrassEffect: return "grass-effect";
        }
    }

    SpaceObjectType fromStringSpaceObjectType(const std::string &input)
    {
        if (input == "character") return SpaceObjectType::Character;
        if (input == "live-photo") return SpaceObjectType::LivePhoto;
        if (input == "live-photo-target") return SpaceObjectType::LivePhotoTarget;
        if (input == "particles") return SpaceObjectType::ParticlesSimple;
        if (input == "placed-item") return SpaceObjectType::PlacedItem;
        if (input == "planet") return SpaceObjectType::Planet;
        if (input == "planet-surface") return SpaceObjectType::PlanetSurface;
        if (input == "ship") return SpaceObjectType::Ship;
        if (input == "star-system") return SpaceObjectType::StarSystem;
        if (input == "space-portal") return SpaceObjectType::SpacePortal;
        if (input == "teleport-clone") return SpaceObjectType::TeleportClone;
        if (input == "grass-effect") return SpaceObjectType::GrassEffect;

        return SpaceObjectType::Unknown;
    }
} // space