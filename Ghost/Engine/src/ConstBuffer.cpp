#include "ConstBuffer.h"

namespace ghost
{
    ConstBuffer::ConstBuffer(unsigned bufferSize, BufferUsage usage, const std::string& name /* = "" */) :
        Buffer(usage, false), _bufferName(name)
    {
        _bufferSize = bufferSize;
    }

    ConstBuffer::~ConstBuffer()
    {

    }
}