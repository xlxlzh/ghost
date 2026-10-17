#include "CommandList.h"

namespace ghost
{
    CommandList::CommandList(CommandListType type) : _type(type)
    {
        
    }

    CommandList::~CommandList()
    {
        
    }

    GraphicsCommandList::GraphicsCommandList() : CommandList(CommandListType::CommandList_Graphics)
    {
        
    }

    GraphicsCommandList::~GraphicsCommandList()
    {
        
    }

    ComputeCommandList::ComputeCommandList() : CommandList(CommandListType::CommandList_Compute)
    {
        
    }

    ComputeCommandList::~ComputeCommandList()
    {
        
    }

    CopyCommandList::CopyCommandList() : CommandList(CommandListType::CommandList_Copy)
    {
        
    }

    CopyCommandList::~CopyCommandList()
    {
        
    }
}
