#include "D3D11Mappings.h"

namespace ghost
{
    D3D11_USAGE D3D11Mappings::getUsage(BufferUsage usage)
    {
        return isDynamic(usage) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    }

    UINT D3D11Mappings::getAccessFlags(BufferUsage usage)
    {
        return isDynamic(usage) ? D3D11_CPU_ACCESS_WRITE : 0;
    }

    bool D3D11Mappings::isDynamic(BufferUsage usage)
    {
        return (usage & BufferUsage::USAGE_DYNAMIC);
    }

    DXGI_FORMAT D3D11Mappings::getFormat(IndexBuffer::IndexType type)
    {
        return type == IndexBuffer::INDEX_32BIT ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
    }

    DXGI_FORMAT D3D11Mappings::getFormat(VertexElementType vElemType)
    {
        switch (vElemType)
        {
        case VET_FLOAT_1:
            return DXGI_FORMAT_R32_FLOAT;
        case VET_FLOAT_2:
            return DXGI_FORMAT_R32G32_FLOAT;
        case VET_FLOAT_3:
            return DXGI_FORMAT_R32G32B32_FLOAT;
        case VET_FLOAT_4:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        }

        //Default float3
        return DXGI_FORMAT_R32G32B32_FLOAT;
    }

    D3D11_PRIMITIVE_TOPOLOGY D3D11Mappings::getPrimitiveType(PrimitiveType pType)
    {
        switch (pType)
        {
        case PRIMITIVE_POINTLIST:
            return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
        case PRIMITIVE_LINELIST:
            return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
        case PRIMITIVE_LINESTRIP:
            return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
        case PRIMITIVE_TRIANGLELIST:
            return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case PRIMITIVE_TRIANGLESTRIP:
            return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        default:
            return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
        }
    }

    const char* D3D11Mappings::getSemanticName(VertexElementSemantic semantic)
    {
        switch (semantic)
        {
        case VES_POSITION:
            return "POSITION";
        case VES_BLEND_WEIGHTS:
            return "BLENDWEIGHT";
        case VES_BLEND_INDICES:
            return "BLENDINDICES";
        case VES_NORMAL:
            return "NORMAL";
        case VES_DIFFUSE:
            return "COLOR";
        case VES_SPECULAR:
            return "COLOR";
        case VES_TEXTURE_COORDINATES:
            return "TEXCOORD";
        case VES_BINORMAL:
            return "BINNORMAL";
        case VES_TANGENT:
            return "TANGENT";
        }

        return "";
    }
}