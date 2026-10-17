#ifndef _D3D12COMMANDLIST_H_
#define _D3D12COMMANDLIST_H_

#include "CommandList.h"

namespace ghost
{
    class GHOST_API D3D12GraphicsCommandList : public GraphicsCommandList
    {
    public:
        D3D12GraphicsCommandList();
        virtual ~D3D12GraphicsCommandList();
        
    };
    
    class GHOST_API D3D12ComputeCommandList : public ComputeCommandList
    {
    public:
        D3D12ComputeCommandList();
        virtual ~D3D12ComputeCommandList();
    };
    
    class GHOST_API D3D12CopyCommandList : public CopyCommandList
    {
    public:
        D3D12CopyCommandList();
        virtual ~D3D12CopyCommandList();
    };
}

#endif