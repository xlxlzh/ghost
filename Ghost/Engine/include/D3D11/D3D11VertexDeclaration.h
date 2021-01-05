#ifndef _D3D11_VERTEXDECLARATION_H_
#define _D3D11_VERTEXDECLARATION_H_

#include "VertexDeclaration.h"
#include "D3D11RenderDevice.h"

namespace ghost
{
    class GHOST_API D3D11VertexDeclaration : public VertexDeclaration
    {
    public:
        D3D11VertexDeclaration(D3D11RenderDevice& device);
        ~D3D11VertexDeclaration();

        void bindToShader();

    protected:


    protected:
        D3D11RenderDevice& _device;
    };

    DECLAR_SMART_POINTER(D3D11VertexDeclaration);
}

#endif
