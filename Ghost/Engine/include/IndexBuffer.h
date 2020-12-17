#ifndef _INDEXBUFFER_H_
#define _INDEXBUFFER_H_

#include "Ghost.h"
#include "Buffer.h"

namespace ghost
{
    class GHOST_API IndexBuffer : public Buffer
    {
    public:
        enum IndexType
        {
            INDEX_16BIT,
            INDEX_32BIT
        };

        IndexBuffer(IndexType type, unsigned numIndex, BufferUsage usage, bool systemMemory);
        ~IndexBuffer();

        IndexType getIndexType() const { return _type; }
        unsigned getNumIndices() const { return _numIndices; }
        unsigned getIndexSize() const { return _indexSize; }

    protected:
        IndexType _type;
        unsigned _numIndices;
        unsigned _indexSize;
    };

    DECLAR_SMART_POINTER(IndexBuffer);
}

#endif // !_VERTEXBUFFER_H_
