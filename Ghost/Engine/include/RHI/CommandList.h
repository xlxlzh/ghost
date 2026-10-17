#ifndef _COMMANDLIST_H_
#define _COMMANDLIST_H_

#include "Ghost.h"

namespace ghost
{
    
    enum class CommandListType
    {
        CommandList_Graphics,
        CommandList_Compute,
        CommandList_Copy
    };
    
    class GHOST_API CommandList
    {
    public:
        CommandList(CommandListType type);
        virtual ~CommandList();
        
        CommandListType GetType() { return _type; }
        
    protected:
        CommandListType _type;
    };
    
    class GHOST_API GraphicsCommandList : public CommandList
    {
    public:
        GraphicsCommandList();
        virtual ~GraphicsCommandList();
    };
    
    class GHOST_API ComputeCommandList : public CommandList
    {
    public:
        ComputeCommandList();
        virtual ~ComputeCommandList();
    };
    
    class GHOST_API CopyCommandList : public CommandList
    {
    public:
        CopyCommandList();
        virtual ~CopyCommandList();
    };
}

#endif
