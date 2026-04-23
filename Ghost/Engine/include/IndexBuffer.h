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

        IndexBuffer(IndexType type, unsigned numIndex, ResourceUsage usage, bool systemMemory);
        ~IndexBuffer();

        IndexType GetIndexType() const { return _type; }
        unsigned GetNumIndices() const { return _numIndices; }
        unsigned GetIndexSize() const { return _indexSize; }

    protected:
        IndexType _type;
        unsigned _numIndices;
        unsigned _indexSize;
    };

    DECLAR_SMART_POINTER(IndexBuffer);
}

#endif // !_VERTEXBUFFER_H_
