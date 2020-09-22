#include "placed_item.hpp"

#include "placeable_item.hpp"
#include "../character.hpp"
#include "../interactions/pickup_action.hpp"
#include "../interactions/use_item_action.hpp"
#include "../../engine.hpp"
#include "../../game_session.hpp"
#include "../../physics/polygon_collider.hpp"
#include "../../utils.hpp"

#include "../../debug/draw_debug.hpp"

namespace space
{
    PlacedItem::PlacedItem(PlaceableItem *item, const sf::Vector2f &position, WalkableArea &area, DrawLayer &onLayer) : PlacedItem(item->id, position, area, onLayer)
    {
        this->item = item;
        _transform.position = position;

        processItem();
    }
    PlacedItem::PlacedItem(const ItemId &itemId, const sf::Vector2f &position, WalkableArea &area, DrawLayer &onLayer) : SpaceObject(Utils::makeObjectId(itemId)),  _collider(nullptr), area(area), onLayer(onLayer), item(nullptr)
    {
        _transform.position = position;
    }
    PlacedItem::~PlacedItem()
    {
        assert(_collider == nullptr);
    }

    void PlacedItem::processItem()
    {
        _sprite.setTexture(*item->definition.texture);
        _sprite.setOrigin(sf::Vector2f(item->definition.texture->getSize()) * 0.5f + item->definition.textureOffset);
        _sprite.setScale(Utils::getInsideScale(), Utils::getInsideScale());

        _interactable.createInteraction<UseItemAction>(this);
        if (item->placeableDefinition.canPickup)
        {
            _interactable.createInteraction<PickupAction>(this);
        }

        auto item = this->item;

        _interactable.setOnPlayerEnters([item](GameSession &session)
        {
            item->onPlayerEnters(session);
        });
        _interactable.setOnPlayerLeaves([item](GameSession &session)
        {
            item->onPlayerLeaves(session);
        });

        _interactable.name(item->definition.name);
    }

    void PlacedItem::addPhysics(b2World &world)
    {
        auto &physicsShape = item->placeableDefinition.physicsShape;
        if (physicsShape.type() == PhysicsShape::Unknown)
        {
            return;
        }

        b2FixtureDef fixtureDef;
        b2CircleShape circleShape;
        b2PolygonShape polygonShape;

        if (physicsShape.type() == PhysicsShape::Circle)
        {
            circleShape.m_radius = physicsShape.radius();
            fixtureDef.shape = &circleShape;
        }
        else if (physicsShape.type() == PhysicsShape::Rectangle)
        {
            polygonShape.SetAsBox(physicsShape.width(), physicsShape.height());
            fixtureDef.shape = &polygonShape;
        }

        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position = b2Vec2(_transform.position.x, _transform.position.y);

        _collider = world.CreateBody(&bodyDef);
        _collider->CreateFixture(&fixtureDef);
        _collider->SetUserData(this);
    }
    void PlacedItem::removePhysics(b2World &world)
    {
        if (_collider == nullptr)
        {
            return;
        }

        world.DestroyBody(_collider);
        _collider = nullptr;
    }

    void PlacedItem::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);
    }

    void PlacedItem::draw(GameSession &session, sf::RenderTarget &target)
    {
        target.draw(_sprite, _worldTransform);
        DrawDebug::glDraw++;

        if (DrawDebug::showPolygons)
        {
            auto &physicsShape = item->placeableDefinition.physicsShape;
            if (physicsShape.type() == PhysicsShape::Circle)
            {
                sf::CircleShape shape;
                shape.setRadius(physicsShape.radius());
                shape.setFillColor(sf::Color(255, 120, 100, 120));
                shape.setPosition(sf::Vector2f(-physicsShape.radius(), -physicsShape.radius()));
                target.draw(shape, _worldTransform);
            }
            else if (physicsShape.type() == PhysicsShape::Rectangle)
            {
                sf::RectangleShape shape;
                shape.setSize(sf::Vector2f(physicsShape.width(), physicsShape.height()));
                shape.setFillColor(sf::Color(120, 255, 100, 120));
                shape.setPosition(sf::Vector2f(-physicsShape.width() * 0.5f, -physicsShape.height() * 0.5f));
                target.draw(shape, _worldTransform);
            }
        }
    }

    void PlacedItem::onPostLoad(GameSession &session, LoadingContext &context)
    {
        if (item == nullptr)
        {
            if (session.tryGetItem(_itemId, &item))
            {
                processItem();
            }
            else
            {
                std::cout << "Unable to find item '" << _itemId << "'for placed item: " << id << std::endl;
            }
        }
    }
}