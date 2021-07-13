#include "placed_item.hpp"

#include "placeable_item.hpp"
#include "../character.hpp"
#include "../area.hpp"
#include "../interactions/pickup_action.hpp"
#include "../interactions/use_item_action.hpp"
#include "../../engine.hpp"
#include "../../game_session.hpp"
#include "../../physics/polygon_collider.hpp"
#include "../../utils.hpp"
#include "../../render_camera.hpp"

#include "../../debug/draw_debug.hpp"

namespace space
{
    const SpaceObjectType PlacedItem::TypeValue = SpaceObjectType::PlacedItem;

    PlacedItem::PlacedItem(PlaceableItem *item) : PlacedItem(item->id)
    {
        this->item = item;

        processItem();
    }

    PlacedItem::PlacedItem(const ItemId &itemId) : SpaceObject(Utils::makeItemId(itemId)), _collider(nullptr)
    {

    }

    PlacedItem::PlacedItem(const ObjectId &id, PlaceableItem *item) : PlacedItem(id, item->id)
    {
        this->item = item;

        processItem();
    }

    PlacedItem::PlacedItem(const ObjectId &id, const ItemId &itemId) : SpaceObject(id), _collider(nullptr)
    {

    }


    PlacedItem::~PlacedItem()
    {

    }

    PlacedItem *PlacedItem::clonePlacedItem(const ObjectId &newId, const CloneContext &context)
    {
        auto result = context.session.createObject<PlacedItem>(newId, item);
        result->transform(_transform);
        return result;
    }

    void PlacedItem::processItem()
    {
        _sprite.setTexture(*item->definition.texture);
        _sprite.setOrigin(sf::Vector2f(item->definition.texture->getSize()) * 0.5f + item->definition.textureOffset);

        _spriteBounds = _sprite.getGlobalBounds();

        createInteractable();
        _interactable->createInteraction<UseItemAction>(this);
        if (item->placeableDefinition.canPickup)
        {
            _interactable->createInteraction<PickupAction>(this);
        }

        auto item = this->item;

        _interactable->setOnCharacterEnters([item](CharacterController *controller, GameSession &session)
        {
            item->onCharacterEnters(controller, session);
        });
        _interactable->setOnCharacterLeaves([item](CharacterController *controller, GameSession &session)
        {
            item->onCharacterLeaves(controller, session);
        });

        _interactable->name(item->definition.name);
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
        _collider->GetUserData().pointer = (uintptr_t)this;
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

    void PlacedItem::insideArea(Area *area)
    {
        if (_insideArea != nullptr)
        {
            auto physicsWorld = _insideArea->physicsWorld();
            if (physicsWorld)
            {
                removePhysics(*physicsWorld);
            }
        }

        SpaceObject::insideArea(area);

        if (area)
        {
            auto physicsWorld = area->physicsWorld();
            if (physicsWorld)
            {
                addPhysics(*physicsWorld);
            }
        }
    }

    void PlacedItem::update(GameSession &session, sf::Time dt, const sf::Transform &parentTransform)
    {
        updateWorldTransform(parentTransform);
    }

    void PlacedItem::draw(GameSession &session, RenderCamera &target)
    {
        target.texture().draw(_sprite, _worldTransform);
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
                target.texture().draw(shape, _worldTransform);
            }
            else if (physicsShape.type() == PhysicsShape::Rectangle)
            {
                sf::RectangleShape shape;
                shape.setSize(sf::Vector2f(physicsShape.width(), physicsShape.height()));
                shape.setFillColor(sf::Color(120, 255, 100, 120));
                shape.setPosition(sf::Vector2f(-physicsShape.width() * 0.5f, -physicsShape.height() * 0.5f));
                target.texture().draw(shape, _worldTransform);
            }
        }
    }

    void PlacedItem::onPostLoad(GameSession &session, LoadingContext &context)
    {
        SpaceObject::onPostLoad(session, context);

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

    bool PlacedItem::doesMouseHover(const Area *inRelationTo, GameSession &session, sf::Vector2f mousePosition) const
    {
        auto worldPos = Utils::getPosition(_worldTransform);
        auto local = mousePosition - worldPos;
        return _spriteBounds.contains(local);
    }

    bool PlacedItem::isInView(const sf::View &view) const
    {
        return Utils::isSpriteInView(_worldTransform, _spriteBounds, view);
    }

    DrawLayers::Type PlacedItem::drawLayer() const
    {
        return item->placeableDefinition.drawLayer;
    }
}