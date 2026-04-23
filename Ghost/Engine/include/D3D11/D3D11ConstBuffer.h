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
        D3D11ConstBuffer(unsigned bufferSize, ResourceUsage usage, D3D11RenderDevice& device, const std::string& name = "");
        ~D3D11ConstBuffer();

        virtual void ReadData(unsigned offset, unsigned length, void* dest) override;
        virtual void WriteData(unsigned offset, unsigned length, const void* src, bool discardBuffer = false) override;

        virtual void* Map(unsigned offset, unsigned length, ResourceLockFlag flag) override;
        virtual void Unmap() override;

        ID3D11Buffer* GetD3DConstBuffer() const;

    protected:
        //Not use two interface, use it at impl.
        virtual void* MapImpl(unsigned offset, unsigned length, ResourceLockFlag flag) override { return nullptr; }
        virtual void UnmapImpl() override { }

    private:
        D3D11Buffer* _buffer;
    };

    DECLAR_SMART_POINTER(D3D11ConstBuffer);
}

#endif
