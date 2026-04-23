#ifndef _D3D11_INDEXBUFFER_H_
#define _D3D11_INDEXBUFFER_H_

#include "IndexBuffer.h"
#include "D3D11Buffer.h"

namespace ghost
{
    class GHOST_API D3D11IndexBuffer : public IndexBuffer
    {
    public:
        D3D11IndexBuffer(IndexType iType, unsigned numIndex, ResourceUsage usage, D3D11RenderDevice& device, bool useSystemMem);
        ~D3D11IndexBuffer();

        virtual void ReadData(unsigned offset, unsigned length, void* dest) override;
        virtual void WriteData(unsigned offset, unsigned length, const void* src, bool discardBuffer = false) override;

        virtual void* Map(unsigned offset, unsigned length, ResourceLockFlag flag) override;
        virtual void Unmap() override;

        bool IsLocked() const;

        ID3D11Buffer* GetD3DIndexBuffer() const;

    protected:
        //Not use two interface, use it at impl.
        virtual void* MapImpl(unsigned offset, unsigned length, ResourceLockFlag flag) override { return nullptr; }
        virtual void UnmapImpl() override { }

    private:
        D3D11Buffer* _buffer;

    };

    DECLAR_SMART_POINTER(D3D11IndexBuffer);
}

#endif
