#include "D3D11VertexBuffer.h"

namespace ghost
{
    D3D11VertexBuffer::D3D11VertexBuffer(unsigned vertexSize, unsigned numVertices, ResourceUsage usage, D3D11RenderDevice& device, bool useSystemMem) :
        VertexBuffer(vertexSize, numVertices, usage, useSystemMem)
    {
        _buffer = new D3D11Buffer(D3D11Buffer::BUFFER_VERTEX, _bufferSize, usage, useSystemMem, device);
    }

    D3D11VertexBuffer::~D3D11VertexBuffer()
    {
        SAFE_DELETE(_buffer);
    }

    bool D3D11VertexBuffer::isLocked() const
    {
        return _buffer->isLocked();
    }

    void* D3D11VertexBuffer::map(unsigned offset, unsigned length, ResourceLockFlag flag)
    {
        return _buffer->map(offset, length, flag);
    }

    void D3D11VertexBuffer::unmap()
    {
        _buffer->unmap();
    }

    void D3D11VertexBuffer::readData(unsigned offset, unsigned length, void* dest)
    {
        _buffer->readData(offset, length, dest);
    }

    void D3D11VertexBuffer::writeData(unsigned offset, unsigned length, const void* src, bool discardBuffer /* = false */)
    {
        _buffer->writeData(offset, length, src, discardBuffer);
    }

    ID3D11Buffer* D3D11VertexBuffer::getD3DVertexBuffer() const
    {
        return _buffer->getD3DBuffer();
    }
}