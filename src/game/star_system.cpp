#include "star_system.hpp"

#include <algorithm>

#include "space_object.hpp"

namespace space
{
    void StarSystem::update(Engine &engine, sf::Time dt)
    {
    }

    void StarSystem::draw(Engine &engine, sf::RenderTarget &target, const sf::Transform &parentTransform)
    {
        for (auto obj : _objects)
        {
            obj->draw(engine, target, parentTransform);
        }
    }

    void StarSystem::addObject(SpaceObject *object)
    {
        if (object == nullptr)
        {
            std::cout << "Cannot add null object to star system" << std::endl;
            return;
        }
        _objects.push_back(object);
    }

    void StarSystem::removeObject(SpaceObject *object)
    {
        if (object == nullptr)
        {
            std::cout << "Cannot remove null object from star system" << std::endl;
            return;
        }

        auto find = std::find(_objects.begin(), _objects.end(), object);
        if (find != _objects.end())
        {
            _objects.erase(find);
        }
        else
        {
            std::cout << "Unable to find object: " << object->id << " to remove from star system" << std::endl;
        }
    }
} // namespace space
