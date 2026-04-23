#include "D3D11ConstBuffer.h"

namespace ghost
{
    D3D11ConstBuffer::D3D11ConstBuffer(unsigned bufferSize, ResourceUsage usage, D3D11RenderDevice& device, const std::string& name /* = "" */) :
        ConstBuffer(bufferSize ,usage, name),
        _buffer(nullptr)
    {
        _buffer = new D3D11Buffer(D3D11Buffer::BUFFER_CONSTANT, bufferSize, usage, false, device);
    }

    D3D11ConstBuffer::~D3D11ConstBuffer()
    {
        SAFE_DELETE(_buffer);
    }

    void* D3D11ConstBuffer::Map(unsigned offset, unsigned length, ResourceLockFlag flag)
    {
        return _buffer->Map(offset, length, flag);
    }

    void D3D11ConstBuffer::Unmap()
    {
        _buffer->Unmap();
    }

    void D3D11ConstBuffer::ReadData(unsigned offset, unsigned length, void* dest)
    {
        _buffer->ReadData(offset, length, dest);
    }

    void D3D11ConstBuffer::WriteData(unsigned offset, unsigned length, const void* src, bool discardBuffer /* = false */)
    {
        _buffer->WriteData(offset, length, src, discardBuffer);
    }

    ID3D11Buffer* D3D11ConstBuffer::GetD3DConstBuffer() const
    {
        return _buffer->GetD3DBuffer();
    }
}