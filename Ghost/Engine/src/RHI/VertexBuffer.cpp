#include "VertexBuffer.h"

namespace ghost
{
    VertexBuffer::VertexBuffer(unsigned vertexSize, unsigned numVertices, ResourceUsage usage, bool systemMemory)
        : Buffer(usage, systemMemory),
        _numVertices(numVertices),
        _vertexSize(vertexSize)
    {
        _bufferSize = numVertices * vertexSize;
    }

    VertexBuffer::~VertexBuffer()
    {

    }
}