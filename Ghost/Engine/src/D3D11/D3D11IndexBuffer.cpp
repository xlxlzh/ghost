#include "D3D11IndexBuffer.h"

namespace ghost
{
    D3D11IndexBuffer::D3D11IndexBuffer(IndexType iType, unsigned numIndex, BufferUsage usage, D3D11RenderDevice& device, bool useSystemMem) :
        IndexBuffer(iType, numIndex, usage, useSystemMem)
    {
        _buffer = new D3D11Buffer(D3D11Buffer::BUFFER_INDEX, numIndex, usage, useSystemMem, device);
    }

    D3D11IndexBuffer::~D3D11IndexBuffer()
    {
        SAFE_DELETE(_buffer);
    }

    bool D3D11IndexBuffer::isLocked() const
    {
        return _buffer->isLocked();
    }

    void* D3D11IndexBuffer::map(unsigned offset, unsigned length, BufferLockFlag flag)
    {
        return _buffer->map(offset, length, flag);
    }

    void D3D11IndexBuffer::unmap()
    {
        _buffer->unmap();
    }

    void D3D11IndexBuffer::readData(unsigned offset, unsigned length, void* dest)
    {
        _buffer->readData(offset, length, dest);
    }

    void D3D11IndexBuffer::writeData(unsigned offset, unsigned length, const void* src, bool discardBuffer /* = false */)
    {
        _buffer->writeData(offset, length, src, discardBuffer);
    }

    ID3D11Buffer* D3D11IndexBuffer::getD3DIndexBuffer() const
    {
        return _buffer->getD3DBuffer();
    }
}