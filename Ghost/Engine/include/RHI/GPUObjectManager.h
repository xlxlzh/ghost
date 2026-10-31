#ifndef _GPUOBJECTMANAGER_H_
#define _GPUOBJECTMANAGER_H_

#include "Ghost.h"
#include "Texture2D.h"
#include "Texture2DRes.h"
#include <unordered_map>
#include "SingleTon.h"

namespace ghost
{
    class GHOST_API GPUObjectManager : public SingleTon<GPUObjectManager>
    {
    public:
        void AddTexture(const Texture2DPtr& texture);
        void RemoveTexture(const Texture2DPtr& texture);
        Texture2DPtr GetOrCreateTexture(const Texture2DResPtr& textureRes);
        
    private:
        std::unordered_map<Texture2DResPtr, Texture2DPtr> _textures;
    };
}

#endif