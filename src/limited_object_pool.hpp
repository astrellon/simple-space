#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

#include "non_copyable.hpp"

namespace space
{
    template <typename T>
    class LimitedObjectPool;

    template <typename T>
    class LimitedObjectPoolEntry : private NonCopyable
    {
        public:
            // Fields
            T *value;

            // Constructor
            LimitedObjectPoolEntry(T *value, LimitedObjectPool<T> &parent) : value(value), _parent(parent) { }
            ~LimitedObjectPoolEntry()
            {
                if (value)
                {
                    _parent.tryFree(value);
                    value = nullptr;
                }
            }

            // Methods
            bool isValid() const { return value != nullptr; }

            operator T *() const { return value; }

        private:
            // Fields
            LimitedObjectPool<T> &_parent;

            // Methods
    };

    // A limited object pool represents just that, a limited supply of pre-defined objects.
    // This is different from a regular object pool that may support creation of new objects
    // as they are needed.
    template <typename T>
    class LimitedObjectPool : private NonCopyable
    {
        public:
            // Definitions
            typedef std::vector<std::unique_ptr<T>> OwnedObjectList;
            typedef std::vector<T *> ObjectList;

            // Fields

            // Constructor

            // Methods
            template <typename... TArgs>
            auto create(TArgs &&... args)
            {
                auto obj = std::make_unique<T>(std::forward<TArgs>(args)...);
                auto result = obj.get();
                _objects.emplace_back(std::move(obj));
                _available.push_back(result);

                return result;
            }

            bool tryGet(T **result)
            {
                if (_available.size() == 0)
                {
                    return false;
                }

                *result = *_available.rbegin();
                _available.pop_back();
                _used.push_back(*result);

                return true;
            }

            LimitedObjectPoolEntry<T> get()
            {
                T *found = nullptr;
                tryGet(&found);
                return LimitedObjectPoolEntry<T>(found, *this);
            }

            bool tryFree(T *object)
            {
                auto find = std::find(_used.begin(), _used.end(), object);
                if (find == _used.end())
                {
                    std::cout << "Attempting to free an object either not in use or not owned" << std::endl;
                    return false;
                }

                _available.push_back(object);
                _used.erase(find);

                return true;
            }

            bool hasAvailable() const { return _available.size() > 0; }

            const OwnedObjectList &objects() const { return _objects; }
            const ObjectList &available() const { return _available; }
            const ObjectList &used() const { return _used; }

        private:
            // Fields
            OwnedObjectList _objects;
            ObjectList _available;
            ObjectList _used;

            // Methods

    };
} // space