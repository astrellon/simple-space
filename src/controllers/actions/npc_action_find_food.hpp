#pragma once

#include <memory>

#include "npc_action.hpp"

#include "../../game/items/placed_item.hpp"

namespace space
{
    class NpcActionMove;
    class FoodItem;

    class NpcActionFindFood : public NpcAction
    {
        public:
            struct FoodSortPair
            {
                float distance;
                PlacedItem *placedFood;

                FoodSortPair(float distance, PlacedItem *placedFood): distance(distance), placedFood(placedFood) { }
            };

            enum class Stage
            {
                LookingForFood, MovingToFood, Eating, Done
            };

            // Fields

            // Constructor
            NpcActionFindFood(NpcController *controller);

            // Methods
            static const std::string ActionType() { return "find-food-action"; }
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
               static bool compareFoodPair(const FoodSortPair &a, const FoodSortPair &b)
               {
                   return a.distance < b.distance;
               }
    };
} // space