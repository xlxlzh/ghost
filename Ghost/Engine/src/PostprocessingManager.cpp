#include "PostprocessingManager.h"

namespace ghost
{
    PostprocessingManager::PostprocessingManager()
    {
        initDefaultEffects();
    }

    void PostprocessingManager::initDefaultEffects()
    {

    }

    bool PostprocessingManager::isEffectEnable(PostprocessingType effect) const
    {
        unsigned flag = static_cast<unsigned>(effect);
        return _postprocessingFlag & flag;
    }

    void PostprocessingManager::enableEffect(PostprocessingType effect)
    {
        _postprocessingFlag |= static_cast<unsigned>(effect);
    }

    void PostprocessingManager::renderAlleffects()
    {
        for (const auto& effect : _effects)
        {
            effect.second->render();
        }
    }
}