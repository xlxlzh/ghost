#ifndef _D3D11_HARDWARESHADER_H_
#define _D3D11_HARDWARESHADER_H_

#include "Shader.h"

namespace ghost
{
    class GHOST_API D3D11HarderwareShader : public Shader
    {
        friend class D3D11RenderDevice;
        friend class D3D11RenderSystem;
    public:
        D3D11HarderwareShader() = default;
        virtual ~D3D11HarderwareShader() { }
    };
}

#endif
