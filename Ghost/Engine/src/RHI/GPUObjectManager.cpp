#include "GPUObjectManager.h"

#include "Engine.h"
#include "RenderDevice.h"

namespace ghost
{
    void GPUObjectManager::AddTexture(const Texture2DPtr& texture)
    {
        
    }
    
    void GPUObjectManager::RemoveTexture(const Texture2DPtr& texture)
    {
        
    }
    
    Texture2DPtr GPUObjectManager::GetOrCreateTexture(const Texture2DResPtr& textureRes)
    {
        if (!textureRes)
            return nullptr;
        
        auto texObj =  _textures.find(textureRes);
        if (texObj == _textures.end())
        {
            RenderDevicePtr renderDevice = Engine::GetInstance()->GetRenderDevice();
            Texture2DPtr rhiTexture = renderDevice->CreateTexture2D(textureRes);
            if (rhiTexture)
                _textures[textureRes] = rhiTexture;
            return rhiTexture;
        }
        
        return texObj->second;
    }
}
