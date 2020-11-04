#pragma once

#include <SFML/System.hpp>

#include "../types.hpp"
#include "character_controller.hpp"
#include "../photo_album.hpp"

namespace space
{
    class PlayerController : public CharacterController
    {
        public:
            // Fields

            // Constructor
            PlayerController(GameSession &session);

            // Methods
            static const std::string ControllerType() { return "player"; }
            virtual std::string type() const { return ControllerType(); }

            virtual void update(sf::Time dt);

            void selectedObject(const ObjectId &id) { _selectedObject = id; }
            ObjectId selectedObject() const { return _selectedObject; }

            PhotoAlbum &photoAlbum() { return _photoAlbum; }
            const PhotoAlbum &photoAlbum() const { return _photoAlbum; }

        private:
            // Fields
            ObjectId _selectedObject;
            PhotoAlbum _photoAlbum;

            // Methods
            void controlShip(sf::Time dt);
            void controlCharacter(sf::Time dt);
    };
} // space