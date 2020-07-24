#ifndef _D3D11_TEXTURE2D_H_
#define _D3D11_TEXTURE2D_H_

#include "Texture2D.h"
#include "D3D11ComPtr.h"

namespace ghost
{
    class GHOST_API D3D11TextureFactory : public ResourceFactory
    {
    public:
        virtual Resource* createResource(const std::string& name, int flags);
        virtual int getType() { return RESOURCE_TEXTURE2D; }
    };

    class GHOST_API D3D11Texture2D : public Texture2D
    {
    public:
        D3D11Texture2D();
        ~D3D11Texture2D();

    private:
        ID3D11Texture2DPtr _texture;
    };

    DECLAR_SMART_POINTER(D3D11Texture2D)
}

#endif
