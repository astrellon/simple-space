#pragma once

#include <queue>

#include <SFML/System.hpp>

#include "character_controller.hpp"
#include "npc_needs.hpp"

namespace space
{
    class Dialogue;
    class Interaction;
    class NPCAction;

    class NpcController : public CharacterController
    {
        public:
            // Fields

            // Constructor
            NpcController(GameSession &session);

            // Methods
            static const std::string ControllerType() { return "npc"; }
            virtual std::string type() const { return ControllerType(); }

            virtual void update(sf::Time dt);

            void dialogue(const Dialogue *dialogue);
            const Dialogue *dialogue() const { return _dialogue; }

        private:
            // Fields
            NPCNeeds _needs;
            std::queue<NPCAction *> _highLevelActions;

            const Dialogue *_dialogue;
            Interaction *_startDialogueAction;
    };
} // space