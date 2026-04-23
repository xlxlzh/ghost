#ifndef _UTILITY_GENERATOR_H_
#define _UTILITY_GENERATOR_H_

#include "Ghost.h"
#include "Model.h"
#include "Vector3.h"

namespace ghost
{
    class GHOST_API UtilityGenerator final
    {
    public:
        static ModelPtr CreateBox(const Vector3f& size);
        static ModelPtr CreateSphere(float radius);
    };
}

#endif
