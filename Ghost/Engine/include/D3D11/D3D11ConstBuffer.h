#ifndef _D3D11_CONSTBUFFER_H_
#define _D3D11_CONSTBUFFER_H_

#include "ConstBuffer.h"
#include "D3D11Buffer.h"

#include <string>

namespace ghost
{
    class GHOST_API D3D11ConstBuffer : public ConstBuffer
    {
    public:
        D3D11ConstBuffer(unsigned bufferSize, BufferUsage usage, D3D11RenderDevice& device, const std::string& name = "");
        ~D3D11ConstBuffer();

        virtual void readData(unsigned offset, unsigned length, void* dest) override;
        virtual void writeData(unsigned offset, unsigned length, const void* src, bool discardBuffer = false) override;

        virtual void* map(unsigned offset, unsigned length, BufferLockFlag flag) override;
        virtual void unmap() override;

        ID3D11Buffer* getD3DConstBuffer() const;

    protected:
        //Not use two interface, use it at impl.
        virtual void* _mapImpl(unsigned offset, unsigned length, BufferLockFlag flag) override { return nullptr; }
        virtual void _unmapImpl() override { }

    private:
        D3D11Buffer* _buffer;
    };

    DECLAR_SMART_POINTER(D3D11ConstBuffer);
}

#endif
