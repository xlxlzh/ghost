#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "Ghost.h"
#include <memory>

namespace ghost
{
    enum BufferLockFlag
    {
        LOCK_NORMAL,
        LOCK_DISCARD,

        LOCK_READ_ONLY,

        LOCK_WRITE_ONLY,
        LOCK_NO_OVERWRITE
    };

    enum BufferUsage
    {
        USAGE_STATIC        = 1,
        USAGE_DYNAMIC       = 2,
        USAGE_WRITE_ONLY    = 4,
        USAGE_DISCARDABLE   = 8,

        USAGE_STATIC_WRITE_ONLY = USAGE_STATIC | USAGE_WRITE_ONLY,

        USAGE_DYNAMIC_WRITE_ONLY = USAGE_DYNAMIC | USAGE_WRITE_ONLY,

        USAGE_DYNAMIC_WRITE_ONLY_DISCARDABLE = USAGE_DYNAMIC_WRITE_ONLY | USAGE_DISCARDABLE,
    };

    class GHOST_API Buffer
    {
    public:
        Buffer(BufferUsage usage, bool systemMemory);
        virtual ~Buffer() { }

        bool isLocked() const { return _isLocked; }
        BufferUsage getUsage() const { return _usage; }
        bool isSystemMemory() const { return _useSystemMemory; }


        virtual void* map(unsigned offset, unsigned length, BufferLockFlag flag);
        virtual void unmap();

        virtual void readData(unsigned offset, unsigned length, void* dest) = 0;
        virtual void writeData(unsigned offset, unsigned length, const void* src, bool discardBuffer = false) = 0;

    protected:
        virtual void* _mapImpl(unsigned offset, unsigned length, BufferLockFlag flag) = 0;
        virtual void _unmapImpl() = 0;

    protected:
        BufferUsage _usage;
        unsigned _bufferSize;

        unsigned _lockStart;
        unsigned _lockSize;

        bool _isLocked;
        bool _useSystemMemory;
    };

    DECLAR_SMART_POINTER(Buffer);
}

#endif
