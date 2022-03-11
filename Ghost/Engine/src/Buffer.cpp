#include "Buffer.h"
#include "LogManager.h"

namespace ghost
{
    Buffer::Buffer(ResourceUsage usage, bool systemMemory) : _usage(usage),
        _useSystemMemory(systemMemory), _bufferSize(0),
        _isLocked(false), _lockSize(0), _lockStart(0)
    {

    }

    void* Buffer::map(unsigned offset, unsigned length, ResourceLockFlag flag)
    {
        assert(!isLocked());

        void* ret = nullptr;
        if (length + offset > _bufferSize)
        {
            GHOST_LOG_FORMAT_ERROR("Buffer::map out of range.");
        }
        else
        {
            ret = _mapImpl(offset, length, flag);
            _isLocked = true;
        }

        _lockStart = offset;
        _lockSize = length;

        return ret;
    }

    void Buffer::unmap()
    {
        assert(isLocked());

        _unmapImpl();
        _isLocked = false;
    }
}