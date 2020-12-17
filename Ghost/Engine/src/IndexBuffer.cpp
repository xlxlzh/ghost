#include "IndexBuffer.h"

namespace ghost
{
    IndexBuffer::IndexBuffer(IndexType type, unsigned numIndex, BufferUsage usage, bool systemMemory) :
        Buffer(usage, systemMemory),
        _type(type), _numIndices(numIndex)
    {
        switch (type)
        {
        case INDEX_16BIT:
            _indexSize = sizeof(unsigned short);
        case INDEX_32BIT:
            _indexSize = sizeof(unsigned int);
            break;
        }
    }

    IndexBuffer::~IndexBuffer()
    {

    }
}