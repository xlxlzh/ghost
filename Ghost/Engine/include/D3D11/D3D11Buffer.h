#ifndef _D3D11_BUFFER_H_
#define _D3D11_BUFFER_H_

#include "Buffer.h"
#include "D3D11ComPtr.h"

namespace ghost
{
    class D3D11RenderDevice;

    class GHOST_API D3D11Buffer : public Buffer
    {
    public:
        enum BufferType
        {
            BUFFER_VERTEX,
            BUFFER_INDEX,
            BUFFER_CONSTANT
        };

    public:
        D3D11Buffer(BufferType buffType, unsigned bufferSize, BufferUsage usage, bool systemMemory, D3D11RenderDevice& device);
        ~D3D11Buffer() { }

        ID3D11Buffer* getD3DBuffer() { return _buffer.Get(); }

        virtual void readData(unsigned offset, unsigned length, void* dest);
        virtual void writeData(unsigned offset, unsigned length, const void* src, bool discardBuffer = false);

    protected:
        virtual void* _mapImpl(unsigned offset, unsigned length, BufferLockFlag flag) override;
        virtual void _unmapImpl() override;

    protected:
        ID3D11BufferPtr _buffer;
        D3D11_BUFFER_DESC _desc;

        BufferType _bufferType;

        D3D11RenderDevice& _device;
    };

    DECLAR_SMART_POINTER(D3D11Buffer);
}

#endif
