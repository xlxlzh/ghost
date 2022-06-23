#ifndef _POSTPROCESSING_MANAGER_H_
#define _POSTPROCESSING_MANAGER_H_

#include "Ghost.h"

namespace ghost
{
    enum class PostprocessingType : unsigned int
    {
        POSTPROCESSING_NONE             = 0x00000000,
        POSTPROCESSING_BLOOM            = 0x00000001,
        POSTPROCESSING_TONEMAPPING      = 0x00000002,
    };

    class GHOST_API PostprocessingManager
    {

    };
}

#endif
