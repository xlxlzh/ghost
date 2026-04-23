#ifndef _POSTPROCESSING_MANAGER_H_
#define _POSTPROCESSING_MANAGER_H_

#include "Ghost.h"
#include "SingleTon.h"
#include <unordered_map>

namespace ghost
{
    enum class PostprocessingType : unsigned int
    {
        POSTPROCESSING_NONE             = 0x00000000,
        POSTPROCESSING_BLOOM            = 0x00000001,
        POSTPROCESSING_TONEMAPPING      = 0x00000002,
    };

    class GHOST_API Postprocessing
    {
    public:
        virtual void Render() = 0;
    };

    DECLAR_SMART_POINTER(Postprocessing);

    class GHOST_API PostprocessingManager : public SingleTon<PostprocessingManager>
    {
    public:
        PostprocessingManager();

        void InitDefaultEffects();
        bool IsEffectEnable(PostprocessingType effect) const;
        void EnableEffect(PostprocessingType effect);

        void RenderAlleffects();

    private:
        unsigned _postprocessingFlag;

        std::unordered_map<unsigned, PostprocessingPtr> _effects;
    };
}

#endif
