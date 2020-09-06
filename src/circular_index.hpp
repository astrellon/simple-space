#pragma once

#include <stddef.h>

namespace space
{
    class CircularIndex
    {
        public:
            // Fields

            // Constructor
            CircularIndex(int maxIndex) : _index(0), _maxIndex(maxIndex) { }
            CircularIndex(int maxIndex, int index) : _index(index), _maxIndex(maxIndex) { }

            // Methods
            inline CircularIndex& operator++()
            {
                ++_index;
                if (_index >= _maxIndex) { _index = 0; }

                return *this;
            }

            inline CircularIndex& operator--()
            {
                --_index;
                if (_index <= 0) { _index = _maxIndex - 1; }

                return *this;
            }

            inline bool operator==(const CircularIndex &rhs) const
            {
                return _index == rhs._index;
            }
            inline bool operator!=(const CircularIndex &rhs) const
            {
                return _index != rhs._index;
            }

            operator int() const { return _index; }
            operator size_t() const { return static_cast<size_t>(_index); }

        private:
            int _index;
            int _maxIndex;
    };
} // space