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

    bool D3D11VertexBuffer::IsLocked() const
    {
        return _buffer->IsLocked();
    }

    void* D3D11VertexBuffer::Map(unsigned offset, unsigned length, ResourceLockFlag flag)
    {
        return _buffer->Map(offset, length, flag);
    }

    void D3D11VertexBuffer::Unmap()
    {
        _buffer->Unmap();
    }

    void D3D11VertexBuffer::ReadData(unsigned offset, unsigned length, void* dest)
    {
        _buffer->ReadData(offset, length, dest);
    }

    void D3D11VertexBuffer::WriteData(unsigned offset, unsigned length, const void* src, bool discardBuffer /* = false */)
    {
        _buffer->WriteData(offset, length, src, discardBuffer);
    }

    ID3D11Buffer* D3D11VertexBuffer::GetD3DVertexBuffer() const
    {
        return _buffer->GetD3DBuffer();
    }
}