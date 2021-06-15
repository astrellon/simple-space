#pragma once

#include <memory>

#include "npc_action.hpp"

#include "../../game/items/placed_item.hpp"

namespace space
{
    class NpcActionMove;
    class BedItem;

    class NpcActionFindBed : public NpcAction
    {
        public:
            struct BedSortPair
            {
                float distance;
                PlacedItem *placedBed;

                BedSortPair(float distance, PlacedItem *placedBed): distance(distance), placedBed(placedBed) { }
            };

            enum class Stage
            {
                LookingForBed, MovingToBed, Sleeping, Done
            };

            // Fields

            // Constructor
            NpcActionFindBed(NpcController *controller);

            // Methods
            static const std::string ActionType() { return "find-bed-action"; }
            virtual std::string type() const { return ActionType(); }

            virtual void update(sf::Time dt);
            virtual bool isComplete() const { return _stage == Stage::Done; }

            virtual void onComplete();

            Stage stage() const { return _stage; }

           private:
               // Fields
               Stage _stage;
               std::unique_ptr<NpcActionMove> _moveStage;
               ObjectId _destinationId;

               // Methods
               static bool compareBedPair(const BedSortPair &a, const BedSortPair &b)
               {
                   return a.distance < b.distance;
               }
    };
} // space