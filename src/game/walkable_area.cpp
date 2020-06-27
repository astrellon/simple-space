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

    void WalkableArea::update(Engine &engine, sf::Time dt)
    {

    }

    void WalkableArea::draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        for (auto &placedItem : _placedItems)
        {
            placedItem.draw(target, parentTransform);
        }

        for (auto &character : _characters)
        {
            character->draw(engine, target, parentTransform);
        }
    }

    void WalkableArea::addCharacter(Character *character)
    {
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
