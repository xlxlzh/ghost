#ifndef _D3D11_MAPPINGS_H_
#define _D3D11_MAPPINGS_H_

#include "Ghost.h"
#include "D3D11ComPtr.h"
#include "Buffer.h"
#include "IndexBuffer.h"
#include "RenderSystem.h"

namespace ghost
{
    class GHOST_API D3D11Mappings
    {
    public:
        static D3D11_USAGE getUsage(BufferUsage usage);
        static UINT getAccessFlags(BufferUsage usage);
        static bool isDynamic(BufferUsage usage);

        static DXGI_FORMAT getFormat(IndexBuffer::IndexType type);
        static DXGI_FORMAT getFormat(VertexElementType vElemType);
        static DXGI_FORMAT getFormat(GhostColorFormat fmt);

        static D3D11_PRIMITIVE_TOPOLOGY getPrimitiveType(PrimitiveType pType);

        static const char* getSemanticName(VertexElementSemantic semantic);

        static D3D11_CULL_MODE getCullMode(CullMode cull);
        static D3D11_FILL_MODE getFillMode(FillMode fill);
        static D3D11_COMPARISON_FUNC getComparison(CompareFunction func);
    };
}

#endif
