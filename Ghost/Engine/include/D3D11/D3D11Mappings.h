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
        static D3D11_USAGE getUsage(ResourceUsage usage);
        static UINT getAccessFlags(ResourceUsage usage);
        static bool isDynamic(ResourceUsage usage);

        static DXGI_FORMAT getFormat(IndexBuffer::IndexType type);
        static DXGI_FORMAT getFormat(VertexElementType vElemType);
        static DXGI_FORMAT getFormat(GhostColorFormat fmt);

        static D3D11_PRIMITIVE_TOPOLOGY getPrimitiveType(PrimitiveType pType);

        static const char* getSemanticName(VertexElementSemantic semantic);

        static D3D11_CULL_MODE getCullMode(CullMode cull);
        static D3D11_FILL_MODE getFillMode(FillMode fill);
        static D3D11_COMPARISON_FUNC getComparison(CompareFunction func);

        static D3D11_TEXTURE_ADDRESS_MODE getAddressMode(TextureAddressingMode mode);
        static D3D11_FILTER getFilter(const FilterOptions min, const FilterOptions mag, const FilterOptions mip);
    };
}

#endif
