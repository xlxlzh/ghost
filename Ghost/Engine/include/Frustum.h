#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "Ghost.h"
#include "Plane.h"

#include <array>

namespace ghost
{
    class GHOST_API Frustum
    {
    public:
        Frustum();
        ~Frustum();

    private:
        std::array<Plane, 6> _planes;
    };
}

#endif
