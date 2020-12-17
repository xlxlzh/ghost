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
}