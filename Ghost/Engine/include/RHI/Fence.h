#ifndef _FENCE_H_
#define _FENCE_H_

#include "Ghost.h"
#include "RenderDevice.h"

namespace ghost
{
    class GHOST_API Fence
    {
    public:
        Fence() = default;
        virtual ~Fence() = default;
        
        virtual bool Create(const RenderDevicePtr& device) { return true; }
    };
    
    DECLAR_SMART_POINTER(Fence)
}

#endif
