#ifndef _VERTEXBUFFER_H_
#define _VERTEXBUFFER_H_

#include "Ghost.h"
#include "Buffer.h"

namespace ghost
{
    class GHOST_API VertexBuffer : public Buffer
    {
    public:
        VertexBuffer(unsigned vertexSize, unsigned numVertices, ResourceUsage usage, bool systemMemory);
        ~VertexBuffer();

        unsigned getVertexSize() const { return _vertexSize; }
        unsigned getNumVertices() const { return _numVertices; }

    protected:
        unsigned _numVertices;
        unsigned _vertexSize;
    };

    DECLAR_SMART_POINTER(VertexBuffer);
}

#endif // !_VERTEXBUFFER_H_
