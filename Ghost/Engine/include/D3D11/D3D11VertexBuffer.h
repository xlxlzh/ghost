#ifndef _D3D11_VERTEXBUFFER_H_
#define _D3D11_VERTEXBUFFER_H_

#include "VertexBuffer.h"
#include "D3D11Buffer.h"

namespace ghost
{
    class GHOST_API D3D11VertexBuffer : public VertexBuffer
    {
    public:
        D3D11VertexBuffer(unsigned vertexSize, unsigned numVertices, ResourceUsage usage, D3D11RenderDevice& device, bool useSystemMem);
        ~D3D11VertexBuffer();

        virtual void readData(unsigned offset, unsigned length, void* dest) override;
        virtual void writeData(unsigned offset, unsigned length, const void* src, bool discardBuffer = false) override;

        virtual void* map(unsigned offset, unsigned length, ResourceLockFlag flag) override;
        virtual void unmap() override;

        bool isLocked() const;

        ID3D11Buffer* getD3DVertexBuffer() const;

    protected:
        //Not use two interface, use it at impl.
        virtual void* _mapImpl(unsigned offset, unsigned length, ResourceLockFlag flag) override { return nullptr; }
        virtual void _unmapImpl() override { }

    private:
        D3D11Buffer* _buffer;

    };

    DECLAR_SMART_POINTER(D3D11VertexBuffer)
}

#endif
