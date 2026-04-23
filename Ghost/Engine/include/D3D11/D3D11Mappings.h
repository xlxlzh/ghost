#ifndef _D3D11_MAPPINGS_H_
#define _D3D11_MAPPINGS_H_

#include "Ghost.h"
#include "D3D11ComPtr.h"
#include "Buffer.h"
#include "IndexBuffer.h"
#include "RenderSystem.h"
#include "RenderCommon.h"

namespace ghost
{
    class GHOST_API D3D11Mappings
    {
    public:
        static D3D11_USAGE GetUsage(ResourceUsage usage);
        static UINT GetAccessFlags(ResourceUsage usage);
        static bool IsDynamic(ResourceUsage usage);

        static DXGI_FORMAT GetFormat(IndexBuffer::IndexType type);
        static DXGI_FORMAT GetFormat(VertexElementType vElemType);
        static DXGI_FORMAT GetFormat(GhostColorFormat fmt);

        static D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType(PrimitiveType pType);

        static const char* GetSemanticName(VertexElementSemantic semantic);

        static D3D11_CULL_MODE GetCullMode(CullMode cull);
        static D3D11_FILL_MODE GetFillMode(FillMode fill);
        static D3D11_COMPARISON_FUNC GetComparison(CompareFunction func);

        static D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(TextureAddressingMode mode);
        static D3D11_FILTER GetFilter(const FilterOptions min, const FilterOptions mag, const FilterOptions mip);
    };
}

#endif
