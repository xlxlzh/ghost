#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "Ghost.h"
#include "GPUResourceFlags.h"
#include <memory>

namespace ghost
{
    class GHOST_API Buffer
    {
    public:
        Buffer(ResourceUsage usage, bool systemMemory);
        virtual ~Buffer() { }

        bool isLocked() const { return _isLocked; }
        ResourceUsage getUsage() const { return _usage; }
        bool isSystemMemory() const { return _useSystemMemory; }


        virtual void* map(unsigned offset, unsigned length, ResourceLockFlag flag);
        virtual void unmap();

        virtual void readData(unsigned offset, unsigned length, void* dest) = 0;
        virtual void writeData(unsigned offset, unsigned length, const void* src, bool discardBuffer = false) = 0;

    protected:
        virtual void* _mapImpl(unsigned offset, unsigned length, ResourceLockFlag flag) = 0;
        virtual void _unmapImpl() = 0;

    protected:
        ResourceUsage _usage;
        unsigned _bufferSize;

        unsigned _lockStart;
        unsigned _lockSize;

        bool _isLocked;
        bool _useSystemMemory;
    };

    DECLAR_SMART_POINTER(Buffer);
}

#endif
