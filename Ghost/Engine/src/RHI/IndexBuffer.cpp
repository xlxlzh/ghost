#include "IndexBuffer.h"

namespace ghost
{
    IndexBuffer::IndexBuffer(IndexType type, unsigned numIndex, ResourceUsage usage, bool systemMemory) :
        Buffer(usage, systemMemory),
        _type(type), _numIndices(numIndex)
    {
        switch (type)
        {
        case INDEX_16BIT:
            _indexSize = sizeof(unsigned short);
            break;
        case INDEX_32BIT:
            _indexSize = sizeof(unsigned int);
            break;
        }

        _bufferSize = numIndex * _indexSize;
    }

    IndexBuffer::~IndexBuffer()
    {

    }
}