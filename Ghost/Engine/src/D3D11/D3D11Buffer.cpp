#include "D3D11Buffer.h"
#include "D3D11Mappings.h"
#include "D3D11RenderDevice.h"

#include "LogManager.h"
#include "Mesh.h"

namespace ghost
{
    D3D11Buffer::D3D11Buffer(BufferType buffType, unsigned bufferSize, BufferUsage usage, bool systemMemory, D3D11RenderDevice& device) :
        Buffer(usage, systemMemory),
        _device(device),
        _bufferType(buffType)
    {
        _bufferSize = bufferSize;

        _desc.ByteWidth = _bufferSize;
        _desc.CPUAccessFlags = D3D11Mappings::getAccessFlags(usage);
        _desc.MiscFlags = 0;

        if (_useSystemMemory)
        {
            _desc.Usage = D3D11_USAGE_STAGING;
            _desc.BindFlags = 0;
            _desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
        }
        else
        {
            _desc.Usage = D3D11Mappings::getUsage(usage);
            _desc.BindFlags = _bufferType == BUFFER_VERTEX ? D3D11_BIND_VERTEX_BUFFER :
                              _bufferType == BUFFER_INDEX ? D3D11_BIND_INDEX_BUFFER :
                                                            D3D11_BIND_CONSTANT_BUFFER;
        }

        ID3D11DevicePtr d3device = _device.getDevice();
        HRESULT hr = d3device->CreateBuffer(&_desc, nullptr, _buffer.ReleaseAndGetAddressOf());
        if (FAILED(hr))
        {
            GHOST_LOG_FORMAT_ERROR("Create D3D11 buffer failed.");
        }
    }

    void* D3D11Buffer::_mapImpl(unsigned offset, unsigned length, BufferLockFlag flag)
    {
        void* pRet = nullptr;
        if (length > _bufferSize)
        {
            _desc.ByteWidth = _bufferSize;
            
            ID3D11DevicePtr d3ddevice = _device.getDevice();
            HRESULT hr = d3ddevice->CreateBuffer(&_desc, nullptr, _buffer.ReleaseAndGetAddressOf());
            if (FAILED(hr))
            {
                GHOST_LOG_FORMAT_ERROR("Create D3D11 buffer failed.");
            }
        }

        if (_useSystemMemory ||
            (_usage & BufferUsage::USAGE_DYNAMIC && (flag == BufferLockFlag::LOCK_DISCARD || flag == BufferLockFlag::LOCK_NO_OVERWRITE)))
        {
            D3D11_MAP mapType;
            switch (flag)
            {
            case LOCK_DISCARD:
                mapType = _useSystemMemory ? D3D11_MAP_WRITE : D3D11_MAP_WRITE_DISCARD;
                break;
            case LOCK_NO_OVERWRITE:
                mapType = _useSystemMemory ? D3D11_MAP_WRITE : D3D11_MAP_WRITE_NO_OVERWRITE;
                break;
            case LOCK_NORMAL:
                mapType = (_desc.CPUAccessFlags & D3D11_CPU_ACCESS_READ) ? D3D11_MAP_READ_WRITE : D3D11_MAP_WRITE;
                break;
            case LOCK_WRITE_ONLY:
                mapType = D3D11_MAP_WRITE;
                break;
            case LOCK_READ_ONLY:
                mapType = D3D11_MAP_READ;
                break;
            }

            D3D11_MAPPED_SUBRESOURCE mapSub;
            mapSub.pData = nullptr;
            
            ID3D11DeviceContextPtr context = _device.getContext();
            HRESULT hr = context->Map(_buffer.Get(), 0, mapType, 0, &mapSub);
            if (FAILED(hr))
            {
                GHOST_LOG_FORMAT_ERROR("D3D11Buffer map failed.");
            }

            pRet = static_cast<void*>(static_cast<char*>(mapSub.pData) + offset);
        }
        else
        {
            //TODO
        }

        return pRet;
    }

    void D3D11Buffer::_unmapImpl()
    {
        ID3D11DeviceContextPtr context = _device.getContext();
        context->Unmap(_buffer.Get(), 0);
    }

    void D3D11Buffer::writeData(unsigned offset, unsigned length, const void* src, bool discardBuffer /* = false */)
    {
        void* pData = map(offset, length, discardBuffer ? BufferLockFlag::LOCK_DISCARD : BufferLockFlag::LOCK_NORMAL);
        memcpy(pData, src, length);
        unmap();
    }

    void D3D11Buffer::readData(unsigned offset, unsigned length, void* dest)
    {
        void* pData = map(offset, length, BufferLockFlag::LOCK_READ_ONLY);
        memcpy(dest, pData, length);
        unmap();
    }
}