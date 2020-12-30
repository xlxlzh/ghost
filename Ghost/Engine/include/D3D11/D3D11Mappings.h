#ifndef _D3D11_MAPPINGS_H_
#define _D3D11_MAPPINGS_H_

#include "Ghost.h"
#include "D3D11ComPtr.h"
#include "Buffer.h"

namespace ghost
{
    class GHOST_API D3D11Mappings
    {
    public:
        static D3D11_USAGE getUsage(BufferUsage usage);
        static UINT getAccessFlags(BufferUsage usage);
        static bool isDynamic(BufferUsage usage);

        static DXGI_FORMAT getFormat(IndexBuffer::IndexType type);
    };
}

#endif
