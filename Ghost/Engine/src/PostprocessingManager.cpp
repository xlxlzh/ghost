#include "PostprocessingManager.h"

namespace ghost
{
    bool PostprocessingManager::isEffectEnable(PostprocessingType effect) const
    {
        unsigned flag = static_cast<unsigned>(effect);
        return _postprocessingFlag & flag;
    }
}