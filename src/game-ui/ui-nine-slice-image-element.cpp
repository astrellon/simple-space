#include "ui-nine-slice-image-element.hpp"

#include "../render_camera.hpp"

namespace space
{
    void UINineSliceImageElement::drawSelf(Engine &engine, RenderCamera &target)
    {
        _nineSlice.width((int)layoutWidth());
        _nineSlice.height((int)layoutHeight());
        target.texture().draw(_nineSlice, _transform);
    }
} // space