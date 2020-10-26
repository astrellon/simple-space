#pragma once

namespace space
{
    class Area;

    class IHasArea
    {
        public:
            // Fields

            // Constructor

            // Methods
            virtual Area &area() = 0;
            virtual const Area &area() const = 0;
    };
} // space