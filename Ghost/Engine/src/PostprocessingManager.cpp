#include "PostprocessingManager.h"

namespace ghost
{
    PostprocessingManager::PostprocessingManager()
    {
        InitDefaultEffects();
    }

    void PostprocessingManager::InitDefaultEffects()
    {

    }

    bool PostprocessingManager::IsEffectEnable(PostprocessingType effect) const
    {
        unsigned flag = static_cast<unsigned>(effect);
        return _postprocessingFlag & flag;
    }

    void PostprocessingManager::EnableEffect(PostprocessingType effect)
    {
        _postprocessingFlag |= static_cast<unsigned>(effect);
    }

    void PostprocessingManager::RenderAlleffects()
    {
        for (const auto& effect : _effects)
        {
            effect.second->Render();
        }
    }
}