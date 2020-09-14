#pragma once

#include <SFML/Graphics.hpp>
#include "../game/space_object.hpp"

namespace space
{
    class GameSession;

    class GrassEffect : public SpaceObject
    {
        public:
            // Fields

            // Constructor
            GrassEffect(const ObjectId &id);

            // Methods
            virtual void update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform);
            virtual void draw(GameSession &session, sf::RenderTarget &target);

            void texture(const sf::Texture &texture) { _sprite.setTexture(texture, true); }
            const sf::Texture *texture() const { return _sprite.getTexture(); }

            static const std::string SpaceObjectType() { return "grass-effect"; }
            virtual std::string type() const { return SpaceObjectType(); }

        private:
            // Fields
            sf::Sprite _sprite;
            sf::Shader *_shader;
            bool _init;

            // Methods
            void init(GameSession &session);
    };
} // space