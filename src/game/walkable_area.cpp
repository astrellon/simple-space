#include "walkable_area.hpp"

#include "engine.hpp"
#include "game/placeable_item.hpp"
#include "game/character.hpp"

namespace space
{
    PlacedItem::PlacedItem(PlaceableItem const &item, const sf::Vector2f &position) :
        item(item), position(position), _sprite(*item.definition.texture)
    {
        _sprite.setPosition(position);
    }

    void PlacedItem::draw(sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        target.draw(_sprite, parentTransform);
    }



    void WalkableArea::update(Engine &engine, sf::Time dt, const sf::Transform &parentTransform)
    {
        _worldTransform = parentTransform;

        for (auto &character : _characters)
        {
            character->update(engine, dt, parentTransform);
        }
    }

    void WalkableArea::draw(Engine &engine, sf::RenderTarget &target)
    {
        for (auto &placedItem : _placedItems)
        {
            placedItem.draw(target, _worldTransform);
        }

        for (auto &character : _characters)
        {
            character->draw(engine, target);
        }
    }

    void WalkableArea::addCharacter(Character *character)
    {
        character->transform().scale = 0.2f;
        _characters.push_back(character);
    }
    void WalkableArea::removeCharacter(Character *character)
    {
        auto find = std::find(_characters.begin(), _characters.end(), character);
        if (find != _characters.end())
        {
            _characters.erase(find);
        }
    }
} // namespace space
