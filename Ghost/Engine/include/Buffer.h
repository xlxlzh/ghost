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

        bool IsLocked() const { return _isLocked; }
        ResourceUsage GetUsage() const { return _usage; }
        bool IsSystemMemory() const { return _useSystemMemory; }


        virtual void* Map(unsigned offset, unsigned length, ResourceLockFlag flag);
        virtual void Unmap();

        virtual void ReadData(unsigned offset, unsigned length, void* dest) = 0;
        virtual void WriteData(unsigned offset, unsigned length, const void* src, bool discardBuffer = false) = 0;

    protected:
        virtual void* MapImpl(unsigned offset, unsigned length, ResourceLockFlag flag) = 0;
        virtual void UnmapImpl() = 0;

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
