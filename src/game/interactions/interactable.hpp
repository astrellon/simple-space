#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "../../non_copyable.hpp"
#include "interaction.hpp"

namespace space
{
    class SpaceObject;
    class CharacterController;

    class Interactable : private NonCopyable
    {
        public:
            typedef std::vector<std::unique_ptr<Interaction>> InteractionList;
            typedef std::function<void(CharacterController *controller, GameSession &)> InteractCallback;

            // Fields

            // Constructor
            Interactable(SpaceObject *parentObject);

            // Methods
            template <typename T, typename... TArgs>
            auto createInteraction(TArgs &&... args)
            {
                auto interaction = std::make_unique<T>(std::forward<TArgs>(args)...);
                auto result = interaction.get();
                _interactions.emplace_back(std::move(interaction));

                return result;
            }

            bool removeInteraction(Interaction *interaction);

            void onCharacterEnters(CharacterController *controller, GameSession &session);
            void onCharacterLeaves(CharacterController *controller, GameSession &session);

            void setOnCharacterEnters(InteractCallback callback) { _onCharacterEnters = callback; }
            void setOnCharacterLeaves(InteractCallback callback) { _onCharacterLeaves = callback; }

            void name(const std::string &name) { _name = name; }
            const std::string &name() const { return _name; }

            const InteractionList &interactions() const { return _interactions; }
            SpaceObject *parentObject() const { return _parentObject; }

            bool isPlayerInRange() const { return _playerInRange; }

        private:
            // Fields
            InteractionList _interactions;
            SpaceObject *_parentObject;
            InteractCallback _onCharacterEnters;
            InteractCallback _onCharacterLeaves;
            std::string _name;
            bool _playerInRange;
    };
} // space