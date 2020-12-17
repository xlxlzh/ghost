#include "Engine.h"
#include "LogManager.h"
#include "D3D11HardwareShader.h"
#include "D3D11RenderDevice.h"
#include "D3D11ConstBuffer.h"
#include "D3D11VertexBuffer.h"
#include "D3D11IndexBuffer.h"

#pragma comment(lib, "d3dcompiler.lib")

namespace ghost
{
    bool D3D11RenderDevice::initDevice(bool fullscreen, unsigned msaaCount)
    {
        HRESULT hr = S_OK;

        unsigned createFlag = 0;
#if defined(DEBUG) || defined(_DEBUG)
        createFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE
        };

        unsigned numDriverTypes = ARRAYSIZE(driverTypes);

        D3D_FEATURE_LEVEL featureLevels[] =
        {
            //D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
        };

        unsigned featureNums = ARRAYSIZE(featureLevels);

        D3D_FEATURE_LEVEL featureLevel;
        D3D_DRIVER_TYPE driverType;
        for (unsigned i = 0; i < numDriverTypes; ++i)
        {
            driverType = driverTypes[i];
            hr = D3D11CreateDevice(nullptr, driverType, nullptr, createFlag, featureLevels, featureNums,
                D3D11_SDK_VERSION, _device.GetAddressOf(), &featureLevel, _context.GetAddressOf());

            if (SUCCEEDED(hr))
            {
                break;
            }
        }

        if (FAILED(hr))
        {
            return false;
        }

        _featureLevel = featureLevel;
        _driverType = driverType;

        unsigned msaaQuality = 0;
        hr = _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, msaaCount, &msaaQuality);
        if (FAILED(hr))
        {
            msaaCount = 1;
            msaaQuality = 0;
        }

        _sampleCount = msaaCount;
        _sampleQulity = msaaQuality;
        _fullscreen = fullscreen;

        return _initSwapchain();
    }

    bool D3D11RenderDevice::_initSwapchain()
    {
        HRESULT hr = S_OK;

        _device.As(&_dxgiDevice);

        _dxgiDevice->GetAdapter(_dxgiAdapter.GetAddressOf());

        _dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(_dxgiFactory.GetAddressOf()));

        HWND window = (HWND)Engine::getInstance()->getWindow();

        DXGI_SWAP_CHAIN_DESC swapchainDesc;
        memset(&swapchainDesc, 0, sizeof(DXGI_SWAP_CHAIN_DESC));
        swapchainDesc.BufferCount = 1;
        swapchainDesc.BufferDesc.Width = Engine::getInstance()->getWidth();
        swapchainDesc.BufferDesc.Height = Engine::getInstance()->getHeight();
        swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapchainDesc.Flags = 0;
        swapchainDesc.OutputWindow = window;
        swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapchainDesc.Windowed = !_fullscreen;
        swapchainDesc.SampleDesc.Count = _sampleCount;
        swapchainDesc.SampleDesc.Quality = _sampleQulity - 1;

        hr = _dxgiFactory->CreateSwapChain(_device.Get(), &swapchainDesc, _dxgiSwapchain.GetAddressOf());
        if (FAILED(hr))
        {
            LogManager::getInstance()->logDebug("CreateSwapChain failed.");
            return false;
        }

        hr = _dxgiSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(_defaultRenderTexture.GetAddressOf()));
        if (FAILED(hr))
        {
            return false;
        }

        hr = _device->CreateRenderTargetView(_defaultRenderTexture.Get(), nullptr, _defaultRenderView.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        D3D11_TEXTURE2D_DESC depthDesc;
        depthDesc.ArraySize = 1;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthDesc.CPUAccessFlags = 0;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.Height = Engine::getInstance()->getHeight();
        depthDesc.MipLevels = 1;
        depthDesc.MiscFlags = 0;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.Width = Engine::getInstance()->getWidth();

        if (_sampleCount > 1)
        {
            depthDesc.SampleDesc.Count = _sampleCount;
            depthDesc.SampleDesc.Quality = _sampleQulity - 1;
        }
        else
        {
            depthDesc.SampleDesc.Count = 1;
            depthDesc.SampleDesc.Quality = 0;
        }

        hr = _device->CreateTexture2D(&depthDesc, nullptr, _defaultDepthTexture.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        hr = _device->CreateDepthStencilView(_defaultDepthTexture.Get(), nullptr, _defaultDepthView.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        return true;
    }

    bool D3D11RenderDevice::checkSampleCount(unsigned checkCount)
    {
        unsigned checkQuality = 0;
        _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, checkCount, &checkQuality);
        return checkQuality != 0;
    }

    static const char* ShaderProfile[] =
    {
        "vs_4_0",
        "ps_4_0",
        "gs_4_0",
        "hs_4_0",
        "ds_4_0",
    };

    bool D3D11RenderDevice::compileShader(ShaderType type, const char* entry, const std::unordered_map<std::string, std::string>& defines, ShaderResource& shader)
    {
        if (type == SHADER_NONE || !entry)
            return false;

        unsigned char* rowdata = shader.getRawdata();
        int dataSize = shader.getRawdataSize();

        if (rowdata == nullptr || dataSize <= 0)
            return false;

        std::vector<D3D_SHADER_MACRO> macros{};
        for (const auto& define : defines)
        {
            D3D_SHADER_MACRO macro{ 0 };
            macro.Name = define.first.c_str();
            macro.Definition = define.second.c_str();

            macros.push_back(macro);
        }

        D3D_SHADER_MACRO endMacro{ 0 };
        endMacro.Name = nullptr;
        endMacro.Definition = nullptr;
        macros.push_back(endMacro);

        ID3DBlobPtr byteCode, errorMsg;
        HRESULT hr = D3DCompile(rowdata, dataSize, shader.getName().c_str(), macros.size() > 1 ? &macros.front() : nullptr, nullptr, entry, ShaderProfile[type], 0, 0, byteCode.GetAddressOf(), errorMsg.GetAddressOf());
        if (FAILED(hr) || errorMsg)
        {
            if (errorMsg)
            {
                GHOST_LOG_FORMAT_ERROR("Compile shader[%s] failed: %s", shader.getName(), (char*)errorMsg->GetBufferPointer());
            }   
            else
            {
                GHOST_LOG_FORMAT_ERROR("Compile shader[%s] failed.", shader.getName());
            }
        }

        shader.updateByteCodes(type, (unsigned char*)byteCode->GetBufferPointer(), byteCode->GetBufferSize());

        return true;
    }

    Shader* D3D11RenderDevice::createShader(const ShaderResource* shadersRes)
    {
        if (shadersRes == nullptr)
            return nullptr;

        Shader* hardwareShader = new D3D11HarderwareShader();
        for (int i = 0; i < (int)SHADER_NONE; ++i)
        {
            const ShaderByteCode* byteCode = shadersRes->getByteCodeByType((ShaderType)i);
            if (byteCode && byteCode->ByteCode && byteCode->ByteCodeSize > 0)
            {
                HRESULT hr = S_OK;
                switch (i)
                {
                case SHADER_VS:
                {
                    ID3D11VertexShader* vertexShader = nullptr;
                    _device->CreateVertexShader(byteCode->ByteCode, byteCode->ByteCodeSize, nullptr, &vertexShader);
                    if (SUCCEEDED(hr))
                        hardwareShader->updateRawShaderPointer((ShaderType)i, vertexShader);
                    break;
                }      
                case SHADER_PS:
                {
                    ID3D11PixelShader* pixelShader = nullptr;
                    _device->CreatePixelShader(byteCode->ByteCode, byteCode->ByteCodeSize, nullptr, &pixelShader);
                    if (SUCCEEDED(hr))
                        hardwareShader->updateRawShaderPointer((ShaderType)i, pixelShader);
                    break;
                }
                    
                default:
                    break;
                }
            }
        }

        return hardwareShader;
    }

    ConstBufferPtr D3D11RenderDevice::createConstBuffer(unsigned bufferSize, BufferUsage usage, const std::string& name)
    {
        return std::make_shared<D3D11ConstBuffer>(bufferSize, usage, *this, name);
    }

    IndexBufferPtr D3D11RenderDevice::createIndexBuffer(IndexBuffer::IndexType iType, unsigned numIndexes, BufferUsage usage)
    {
        return std::make_shared<D3D11IndexBuffer>(iType, numIndexes, usage, *this, false);
    }

    VertexBufferPtr D3D11RenderDevice::createVertexBuffer(unsigned VertexSize, unsigned numVertices, BufferUsage usage)
    {
        return std::make_shared<D3D11VertexBuffer>(VertexSize, numVertices, usage, *this, false);
    }
}