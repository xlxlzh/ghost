#ifndef _CONSTBUFFER_H_
#define _CONSTBUFFER_H_

#include "Buffer.h"

#include <string>

namespace ghost
{
    class GHOST_API ConstBuffer : public Buffer
    {
    public:
        ConstBuffer(unsigned bufferSize, BufferUsage usage, const std::string& name = "");
        ~ConstBuffer();

        const std::string& getName() const { return _bufferName; }

    protected:
        std::string _bufferName;
    };

    DECLAR_SMART_POINTER(ConstBuffer);
}

#endif
