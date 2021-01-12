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

    D3D11_CULL_MODE D3D11Mappings::getCullMode(CullMode cull)
    {
        switch (cull)
        {
        case CULL_CW:
            return D3D11_CULL_BACK;
        case CULL_CCW:
            return D3D11_CULL_FRONT;
        case CULL_NONE:
            return D3D11_CULL_NONE;
        }

        return D3D11_CULL_BACK;
    }

    D3D11_FILL_MODE D3D11Mappings::getFillMode(FillMode fill)
    {
        switch (fill)
        {
        case FILL_SOLID:
            return D3D11_FILL_SOLID;
        case FILL_WIREFRAME:
            return D3D11_FILL_WIREFRAME;
        }

        return D3D11_FILL_SOLID;
    }

    D3D11_COMPARISON_FUNC D3D11Mappings::getComparison(CompareFunction func)
    {
        switch (func)
        {
        case COMPARISON_NEVER:
            return D3D11_COMPARISON_NEVER;
        case COMPARISON_ALWAYS:
            return D3D11_COMPARISON_ALWAYS;
        case COMPARISON_LESS:
            return D3D11_COMPARISON_LESS;
        case COMPARISON_LESS_EQUAL:
            return D3D11_COMPARISON_LESS_EQUAL;
        case COMPARISON_EQUAL:
            return D3D11_COMPARISON_EQUAL;
        case COMPARISON_NOT_EQUAL:
            return D3D11_COMPARISON_NOT_EQUAL;
        case COMPARISON_GREATER_EQUAL:
            return D3D11_COMPARISON_GREATER_EQUAL;
        case COMPARISON_GREATER:
            return D3D11_COMPARISON_GREATER;
        }

        return D3D11_COMPARISON_ALWAYS;
    }
}