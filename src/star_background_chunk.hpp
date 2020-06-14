#pragma once

#include <map>

#include "particles.hpp"

namespace space
{
    class StarBackground;

    class StarBackgroundChunk : public Particles
    {
        public:
            // Fields

            // Constructor
            StarBackgroundChunk(Engine &engine, const StarBackground &parent);

            // Methods
            bool isActive() const;

            void reinit();

            void position(sf::Vector2i position) { _position = position; }
            sf::Vector2i position() const { return _position; }

            virtual void onInit();
            virtual void onUpdate(sf::Time dt);
            virtual bool onPreDraw(sf::RenderTarget &target, const sf::Transform &parentTransform);
            virtual void onPostDraw(sf::RenderTarget &target, const sf::Transform &parentTransform);

        private:
            sf::Vector2i _position;
            sf::Time _lastUseTime;
            const StarBackground &_parent;
    };
} // space