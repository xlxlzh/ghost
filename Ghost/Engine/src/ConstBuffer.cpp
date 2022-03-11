#include "ConstBuffer.h"

namespace ghost
{
    ConstBuffer::ConstBuffer(unsigned bufferSize, ResourceUsage usage, const std::string& name /* = "" */) :
        Buffer(usage, false), _bufferName(name)
    {
        _bufferSize = bufferSize;
    }

    ConstBuffer::~ConstBuffer()
    {

    }
}