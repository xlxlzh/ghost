#include "D3D11ConstBuffer.h"

namespace ghost
{
    D3D11ConstBuffer::D3D11ConstBuffer(unsigned bufferSize, BufferUsage usage, D3D11RenderDevice& device, const std::string& name /* = "" */) :
        ConstBuffer(bufferSize ,usage, false),
        _buffer(nullptr)
    {
        _buffer = new D3D11Buffer(D3D11Buffer::BUFFER_CONSTANT, bufferSize, usage, false, device);
    }

    D3D11ConstBuffer::~D3D11ConstBuffer()
    {
        SAFE_DELETE(_buffer);
    }

    void* D3D11ConstBuffer::map(unsigned offset, unsigned length, BufferLockFlag flag)
    {
        return _buffer->map(offset, length, flag);
    }

    void D3D11ConstBuffer::unmap()
    {
        _buffer->unmap();
    }

    void D3D11ConstBuffer::readData(unsigned offset, unsigned length, void* dest)
    {
        _buffer->readData(offset, length, dest);
    }

    void D3D11ConstBuffer::writeData(unsigned offset, unsigned length, const void* src, bool discardBuffer /* = false */)
    {
        _buffer->writeData(offset, length, src, discardBuffer);
    }

    ID3D11Buffer* D3D11ConstBuffer::getD3DConstBuffer() const
    {
        return _buffer->getD3DBuffer();
    }
}