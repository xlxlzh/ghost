#include "D3D11IndexBuffer.h"

namespace ghost
{
    D3D11IndexBuffer::D3D11IndexBuffer(IndexType iType, unsigned numIndex, ResourceUsage usage, D3D11RenderDevice& device, bool useSystemMem) :
        IndexBuffer(iType, numIndex, usage, useSystemMem)
    {
        _buffer = new D3D11Buffer(D3D11Buffer::BUFFER_INDEX, _bufferSize, usage, useSystemMem, device);
    }

    D3D11IndexBuffer::~D3D11IndexBuffer()
    {
        SAFE_DELETE(_buffer);
    }

    bool D3D11IndexBuffer::IsLocked() const
    {
        return _buffer->IsLocked();
    }

    void* D3D11IndexBuffer::Map(unsigned offset, unsigned length, ResourceLockFlag flag)
    {
        return _buffer->Map(offset, length, flag);
    }

    void D3D11IndexBuffer::Unmap()
    {
        _buffer->Unmap();
    }

    void D3D11IndexBuffer::ReadData(unsigned offset, unsigned length, void* dest)
    {
        _buffer->ReadData(offset, length, dest);
    }

    void D3D11IndexBuffer::WriteData(unsigned offset, unsigned length, const void* src, bool discardBuffer /* = false */)
    {
        _buffer->WriteData(offset, length, src, discardBuffer);
    }

    ID3D11Buffer* D3D11IndexBuffer::GetD3DIndexBuffer() const
    {
        return _buffer->GetD3DBuffer();
    }
}