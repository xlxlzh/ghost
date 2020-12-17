#ifndef _D3D11_INDEXBUFFER_H_
#define _D3D11_INDEXBUFFER_H_

#include "IndexBuffer.h"
#include "D3D11Buffer.h"

namespace ghost
{
    class GHOST_API D3D11IndexBuffer : public IndexBuffer
    {
    public:
        D3D11IndexBuffer(IndexType iType, unsigned numIndex, BufferUsage usage, D3D11RenderDevice& device, bool useSystemMem);
        ~D3D11IndexBuffer();

        virtual void readData(unsigned offset, unsigned length, void* dest) override;
        virtual void writeData(unsigned offset, unsigned length, const void* src, bool discardBuffer = false) override;

        virtual void* map(unsigned offset, unsigned length, BufferLockFlag flag) override;
        virtual void unmap() override;

        bool isLocked() const;

        ID3D11Buffer* getD3DIndexBuffer() const;

    protected:
        //Not use two interface, use it at impl.
        virtual void* _mapImpl(unsigned offset, unsigned length, BufferLockFlag flag) override { return nullptr; }
        virtual void _unmapImpl() override { }

    private:
        D3D11Buffer* _buffer;

    };
}

#endif
