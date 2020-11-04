#include "photo_album.hpp"

#include "utils.hpp"

namespace space
{
    void PhotoAlbum::addPhoto(LivePhoto *photo)
    {
        _photos.push_back(photo);
    }

    void PhotoAlbum::removePhoto(LivePhoto *photo)
    {
        Utils::remove(_photos, photo);
    }
} // space