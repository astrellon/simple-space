#pragma once

#include <vector>

#include "non_copyable.hpp"

namespace space
{
    class LivePhoto;

    class PhotoAlbum : private NonCopyable
    {
        public:
            typedef std::vector<LivePhoto *> PhotoList;

            // Fields

            // Constructor

            // Methods
            void addPhoto(LivePhoto *photo);
            void removePhoto(LivePhoto *photo);
            const PhotoList &photos() const { return _photos; }

        private:
            // Fields
            PhotoList _photos;

            // Methods

    };
} // space