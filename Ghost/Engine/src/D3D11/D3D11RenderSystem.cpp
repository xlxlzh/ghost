#include <functional>
#include "D3D11RenderSystem.h"
#include "Engine.h"
#include "D3D11Mappings.h"
#include "D3D11RenderTarget.h"
#include "D3D11DepthStencilTarget.h"
#include "D3D11HardwareShader.h"
#include "D3D11VertexBuffer.h"
#include "D3D11IndexBuffer.h"
#include "D3D11VertexDeclaration.h"
#include "D3D11ConstBuffer.h"
#include "LogManager.h"

namespace ghost
{
    D3D11RenderSystem::D3D11RenderSystem(RenderDevicePtr device) : RenderSystem(device)
    {
        ZeroMemory(&_depthStencilDesc, sizeof(_depthStencilDesc));
        ZeroMemory(&_rasterizer, sizeof(_rasterizer));
        ZeroMemory(&_blendDesc, sizeof(_blendDesc));

        setDepthBufferParams(true, true, COMPARISON_LESS_EQUAL);

        _rasterizer.FillMode = D3D11Mappings::getFillMode(_fillMode);
        _rasterizer.CullMode = D3D11Mappings::getCullMode(_cullingMode);
        _rasterizer.DepthClipEnable = true;
        _rasterizer.MultisampleEnable = true;

        for (unsigned i = 0; i < 8; ++i)
            _textures2DUnits[i] = nullptr;
    }

    bool D3D11RenderSystem::initRenderSystem()
    {
        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);

        _srvFunctionTable[SHADER_VS] = std::bind(&ID3D11DeviceContext::VSSetShaderResources, devicePtr->_context.Get(), std::placeholders::_1, 1, std::placeholders::_2);
        _srvFunctionTable[SHADER_GS] = std::bind(&ID3D11DeviceContext::GSSetShaderResources, devicePtr->_context.Get(), std::placeholders::_1, 1, std::placeholders::_2);
        _srvFunctionTable[SHADER_HS] = std::bind(&ID3D11DeviceContext::HSSetShaderResources, devicePtr->_context.Get(), std::placeholders::_1, 1, std::placeholders::_2);
        _srvFunctionTable[SHADER_DS] = std::bind(&ID3D11DeviceContext::DSSetShaderResources, devicePtr->_context.Get(), std::placeholders::_1, 1, std::placeholders::_2);
        _srvFunctionTable[SHADER_PS] = std::bind(&ID3D11DeviceContext::PSSetShaderResources, devicePtr->_context.Get(), std::placeholders::_1, 1, std::placeholders::_2);

        _samplerFunctionTable[SHADER_VS] = std::bind(&ID3D11DeviceContext::VSSetSamplers, devicePtr->_context.Get(), std::placeholders::_1, 1, std::placeholders::_2);
        _samplerFunctionTable[SHADER_GS] = std::bind(&ID3D11DeviceContext::GSSetSamplers, devicePtr->_context.Get(), std::placeholders::_1, 1, std::placeholders::_2);
        _samplerFunctionTable[SHADER_HS] = std::bind(&ID3D11DeviceContext::HSSetSamplers, devicePtr->_context.Get(), std::placeholders::_1, 1, std::placeholders::_2);
        _samplerFunctionTable[SHADER_DS] = std::bind(&ID3D11DeviceContext::DSSetSamplers, devicePtr->_context.Get(), std::placeholders::_1, 1, std::placeholders::_2);
        _samplerFunctionTable[SHADER_PS] = std::bind(&ID3D11DeviceContext::PSSetSamplers, devicePtr->_context.Get(), std::placeholders::_1, 1, std::placeholders::_2);

        return true;
    }

    D3D11RenderSystem::~D3D11RenderSystem()
    {

    }

    void D3D11RenderSystem::setRenderTarget(RenderTargetPtr rt)
    {
        _activeRenerTarget = rt;

        D3D11RenderTargetPtr targetPtr = GHOST_SMARTPOINTER_CAST(D3D11RenderTarget, rt);
        if (rt && targetPtr)
        {
            ID3D11RenderTargetView* renderViews[GHOST_MAX_RENDERTARGETS] = {0};

            unsigned numViews = targetPtr->getNumOfViews();
            for (unsigned i = 0; i < GHOST_MAX_RENDERTARGETS; ++i)
            {
                renderViews[i] = targetPtr->getRenderTargetViewByIndex(i).Get();
                if (!renderViews[i])
                {
                    break;
                }
            }

            ID3D11DepthStencilView* depthView = nullptr;
            D3D11DepthStencilTargetPtr depthBuffer = GHOST_SMARTPOINTER_CAST(D3D11DepthStencilTarget, rt->getAttachDepthBuffer());
            if (depthBuffer)
            {
                depthView = depthBuffer->getDepthView().Get();
            }

            D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);
            devicePtr->_context->OMSetRenderTargets(numViews, renderViews, depthView);

            const Viewport& vp = rt->getAttachViewport();
            D3D11_VIEWPORT d3d11vp;
            d3d11vp.TopLeftX = vp.getViewportX();
            d3d11vp.TopLeftY = vp.getViewportY();
            d3d11vp.Width = vp.getViewportWidth();
            d3d11vp.Height = vp.getViewportHeight();
            d3d11vp.MinDepth = vp.getMinZ();
            d3d11vp.MaxDepth = vp.getMaxZ();

            devicePtr->_context->RSSetViewports(1, &d3d11vp);
        }
    }

    void D3D11RenderSystem::clearRenderTarget(TargetClear clearFlag /* = CLEAR_ALL */, Color col /* = Color::Black */, float z /* = 1.0 */, unsigned stencil /* = 0.0 */)
    {
        if (clearFlag & TargetClear::CLEAR_COLOR)
        {
            _clearRenderTarget(col);
        }

        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);

        unsigned flags = 0;
        if (clearFlag & TargetClear::CLEAR_DEPTH)
        {
            flags |= D3D11_CLEAR_DEPTH;
        }

        if (clearFlag & TargetClear::CLEAR_STENCIL)
        {
            flags |= D3D11_CLEAR_STENCIL;
        }

        if (_activeRenerTarget)
        {
            D3D11DepthStencilTargetPtr depthPtr = GHOST_SMARTPOINTER_CAST(D3D11DepthStencilTarget, _activeRenerTarget->getAttachDepthBuffer());
            if (depthPtr)
            {
                devicePtr->_context->ClearDepthStencilView(depthPtr->getDepthView().Get(), flags, z, stencil);
            }
        }
        else
        {
            devicePtr->_context->ClearDepthStencilView(devicePtr->_defaultDepthView.Get(), flags, z, stencil);
        }
    }

    void D3D11RenderSystem::_clearRenderTarget(Color cl)
    {
        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);
        if (_activeRenerTarget)
        {
            D3D11RenderTargetPtr targetPtr = GHOST_SMARTPOINTER_CAST(D3D11RenderTarget, _activeRenerTarget);
            if (targetPtr)
            {
                ID3D11RenderTargetView* renderViews[GHOST_MAX_RENDERTARGETS] = { 0 };

                unsigned numViews = targetPtr->getNumOfViews();
                for (unsigned i = 0; i < GHOST_MAX_RENDERTARGETS; ++i)
                {
                    renderViews[i] = targetPtr->getRenderTargetViewByIndex(i).Get();
                    if (!renderViews[i])
                    {
                        break;
                    }
                }

                const float* clearColor = cl.getColorPtr();
                for (unsigned i = 0; i < numViews; ++i)
                {
                    devicePtr->_context->ClearRenderTargetView(renderViews[i], clearColor);
                }
            }
        }
        else
        {
            devicePtr->_context->ClearRenderTargetView(devicePtr->_defaultRenderView.Get(), cl.getColorPtr());
        }
    }

    void D3D11RenderSystem::setVertexBuffer(VertexBufferPtr vBuffer)
    {
        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);

        D3D11VertexBufferPtr vBufferPtr = GHOST_SMARTPOINTER_CAST(D3D11VertexBuffer, vBuffer);
        ID3D11Buffer* buffers[] = {vBufferPtr->getD3DVertexBuffer()};
        UINT strides[] = { vBufferPtr->getVertexSize() };
        UINT offset = 0;

        devicePtr->_context->IASetVertexBuffers(0, 0, buffers, strides, &offset);
    }

    void D3D11RenderSystem::setVertexBufferBinding(VertexBufferBinding* binding)
    {
        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);

        const VertexBufferBindingMap& bindings = binding->getBindings();
        for (auto itBind : bindings)
        {
            D3D11VertexBufferPtr d3d11Buffer = GHOST_SMARTPOINTER_CAST(D3D11VertexBuffer, itBind.second);

            UINT stride = d3d11Buffer->getVertexSize();
            UINT offset = 0;
            UINT slot = itBind.first;

            ID3D11Buffer* vBuffer = d3d11Buffer->getD3DVertexBuffer();
            devicePtr->_context->IASetVertexBuffers(slot, 1, &vBuffer, &stride, &offset);
        }
    }

    void D3D11RenderSystem::setIndexBuffer(IndexBufferPtr iBuffer)
    {
        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);

        D3D11IndexBufferPtr iBufferPtr = GHOST_SMARTPOINTER_CAST(D3D11IndexBuffer, iBuffer);
        devicePtr->_context->IASetIndexBuffer(iBufferPtr->getD3DIndexBuffer(), D3D11Mappings::getFormat(iBufferPtr->getIndexType()), 0);
    }

    void D3D11RenderSystem::setVertexDeclaration(VertexDeclarationPtr vDecl)
    {
        if (_currentMaterial)
        {
            ShaderPass* pass = _currentMaterial->getShaderPass(_currentRenderPass);

            D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);

            D3D11VertexDeclarationPtr d3d11Decl = GHOST_SMARTPOINTER_CAST(D3D11VertexDeclaration, vDecl);

            InputSignatureList* signatures = pass->getShaderInputSignature();
            auto inputIt = _inputlayouts.find(signatures);
            if (inputIt != _inputlayouts.end())
            {
                ID3D11InputLayout* d3d11Input = inputIt->second.Get();
                devicePtr->_context->IASetInputLayout(d3d11Input);
            }
            else
            {
                const auto& elements = d3d11Decl->getElements();

                std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;

                InputSignatureList sigs = *signatures;
                for (auto& matSig : sigs)
                {
                    bool found = false;
                    std::list<VertexElement>::const_iterator itEle;
                    for (itEle = elements.begin(); itEle != elements.end(); ++itEle)
                    {
                        const char* semanticName = D3D11Mappings::getSemanticName(itEle->getSemantic());
                        unsigned semanticIndex = itEle->getIndex();

                        if (strcmp(semanticName, matSig._semantic.c_str()) == 0
                            && semanticIndex == matSig._index)
                        {
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        GHOST_LOG_FORMAT_ERROR("No vertex element for semantic %s%s in shader %s found",
                            matSig._semantic.c_str(), matSig._index, _currentMaterial->getName());
                    }

                    D3D11_INPUT_ELEMENT_DESC elem = {};
                    elem.SemanticName = matSig._semantic.c_str();
                    elem.SemanticIndex = matSig._index;
                    elem.Format = D3D11Mappings::getFormat(itEle->getType());
                    elem.InputSlot = itEle->getStreamSlot();
                    elem.AlignedByteOffset = itEle->getOffset();
                    elem.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
                    elem.InstanceDataStepRate = 0;
                    inputDesc.push_back(elem);
                }

                if (!inputDesc.empty())
                {
                    auto shaderRes = pass->getLinkedShader();
                    auto shaderBytecode = shaderRes->getByteCodeByType(SHADER_VS);

                    ID3D11InputLayoutPtr inputlayoutPtr = nullptr;
                    HRESULT hr = devicePtr->_device->CreateInputLayout(&inputDesc[0], inputDesc.size(),
                        &shaderBytecode->ByteCode[0], shaderBytecode->ByteCodeSize, inputlayoutPtr.ReleaseAndGetAddressOf());
                    if (FAILED(hr))
                    {
                        GHOST_LOG_FORMAT_ERROR("Can not CreateInputLayout");
                    }

                    _inputlayouts[signatures] = inputlayoutPtr;

                    devicePtr->_context->IASetInputLayout(inputlayoutPtr.Get());
                }
            }
        }
    }

    void D3D11RenderSystem::setConstBuffer(ShaderType shaderType, ConstBufferPtr constBuffer)
    {
        if (_currentMaterial == nullptr)
            return;
        
        ShaderPass* pass = _currentMaterial->getShaderPass(_currentRenderPass);
        unsigned slot = pass->getConstBufferSlot(shaderType, constBuffer->getName());

        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);
        D3D11ConstBufferPtr constBufferPtr = GHOST_SMARTPOINTER_CAST(D3D11ConstBuffer, constBuffer);
        ID3D11Buffer* buffers[] = { constBufferPtr->getD3DConstBuffer() };
        switch (shaderType)
        {
        case SHADER_VS:
            devicePtr->_context->VSSetConstantBuffers(slot, 1, buffers);
            break;
        case SHADER_PS:
            devicePtr->_context->PSSetConstantBuffers(slot, 1, buffers);
            break;
        case SHADER_GS:
            devicePtr->_context->GSSetConstantBuffers(slot, 1, buffers);
            break;
        case SHADER_HS:
            devicePtr->_context->HSSetConstantBuffers(slot, 1, buffers);
            break;
        case SHADER_DS:
            devicePtr->_context->DSSetConstantBuffers(slot, 1, buffers);
            break;
        default:
            break;
        }
    }

    void D3D11RenderSystem::setPrimitiveType(PrimitiveType pType)
    {
        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);
        devicePtr->_context->IASetPrimitiveTopology(D3D11Mappings::getPrimitiveType(pType));
    }

    void D3D11RenderSystem::drawPrimitive(unsigned numVertices, unsigned startIndex)
    {
        _updateRenderStateBeforeRendering();

        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);
        devicePtr->_context->Draw(numVertices, startIndex);
    }

    void D3D11RenderSystem::drawPrimitiveIndexed(unsigned numIndices, unsigned indexLocation, int baseVertIndex)
    {
        _updateRenderStateBeforeRendering();

        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);
        devicePtr->_context->DrawIndexed(numIndices, indexLocation, baseVertIndex);
    }

    void D3D11RenderSystem::drawPrimitiveInstance()
    {
        //TODO
        _updateRenderStateBeforeRendering();
    }

    void D3D11RenderSystem::setShader(const Shader* shader)
    {
        if (shader && shader->isValid())
        {
            const D3D11HarderwareShader* d3dshader = dynamic_cast< const D3D11HarderwareShader*>(shader);
            if (d3dshader)
            {
                D3D11RenderDevicePtr device = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);
                if (d3dshader->isShaderTypeValid(SHADER_VS))
                {
                    device->_context->VSSetShader((ID3D11VertexShader*)d3dshader->_shaders[SHADER_VS], nullptr, 0);
                }

                if (d3dshader->isShaderTypeValid(SHADER_PS))
                {
                    device->_context->PSSetShader((ID3D11PixelShader*)d3dshader->_shaders[SHADER_PS], nullptr, 0);
                }
            }
        }
    }

    void D3D11RenderSystem::endScene()
    {
        D3D11RenderDevicePtr d3dDevice = std::dynamic_pointer_cast<D3D11RenderDevice>(Engine::getInstance()->getRenderDevice());
        d3dDevice->_dxgiSwapchain->Present(0, 0);
    }

    void D3D11RenderSystem::setCullMode(CullMode cull)
    {
        _cullingMode = cull;

        _rasterizerDescChagned = true;
        _rasterizer.CullMode = D3D11Mappings::getCullMode(cull);
    }

    void D3D11RenderSystem::setFillMode(FillMode fillMode)
    {
        _fillMode = fillMode;

        _rasterizerDescChagned = true;
        _rasterizer.FillMode = D3D11Mappings::getFillMode(fillMode);
    }

    void D3D11RenderSystem::setSamplerState()
    {
        _samplerStateChanged = true;

    }

    void D3D11RenderSystem::setTexture(ShaderType type, unsigned slot, Texture2DPtr tex2D)
    {
        if (slot >= 8 || type >= SHADER_TYPE_NUM)
            return;

        _textures2DUnits[slot] = GHOST_SMARTPOINTER_CAST(D3D11Texture2D, tex2D);
        if (tex2D != nullptr)
        {
            _srvFunctionTable[type](slot, _textures2DUnits[slot]->getD3D11ShaderResourceView().GetAddressOf());
        }
        else
        {
            ID3D11ShaderResourceView* views[]{ nullptr };
            _srvFunctionTable[type](slot, views);
        }
    }

    void D3D11RenderSystem::setDepthBufferParams(bool depthTest, bool depthWrite, CompareFunction depthFunction)
    {
        setDepthTestEnable(depthTest);
        setDepthWriteEnable(depthWrite);
        setDepthFunction(depthFunction);
    }

    void D3D11RenderSystem::setDepthTestEnable(bool enable)
    {
        _depthStencilDesc.DepthEnable = enable;
        _depthStencilDescChanged = true;
    }

    void D3D11RenderSystem::setDepthWriteEnable(bool enable)
    {
        if (enable)
        {
            _depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        }
        else
        {
            _depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        }

        _depthStencilDescChanged = true;
    }

    void D3D11RenderSystem::setDepthFunction(CompareFunction fun)
    {
        _depthStencilDesc.DepthFunc = D3D11Mappings::getComparison(fun);
        _depthStencilDescChanged = true;
    }

    void D3D11RenderSystem::setColorBufferEnable(bool r, bool g, bool b, bool a)
    {
        UINT8 val = 0;
        if (r)
            val |= D3D11_COLOR_WRITE_ENABLE_RED;
        if (g)
            val |= D3D11_COLOR_WRITE_ENABLE_GREEN;
        if (b)
            val |= D3D11_COLOR_WRITE_ENABLE_BLUE;
        if (a)
            val |= D3D11_COLOR_WRITE_ENABLE_ALPHA;

        _blendDesc.RenderTarget[0].RenderTargetWriteMask = val;
        _blendDescChanged = true;
    }

    void D3D11RenderSystem::useDefaultRenderTarget()
    {
        D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);

        D3D11_VIEWPORT vp;
        vp.Width = devicePtr->_width;
        vp.Height = devicePtr->_height;
        vp.TopLeftX = 0.0;
        vp.TopLeftY = 0.0;
        vp.MinDepth = 0.0;
        vp.MaxDepth = 1.0;

        devicePtr->_context->RSSetViewports(1, &vp);

        devicePtr->_context->OMSetRenderTargets(1, devicePtr->_defaultRenderView.GetAddressOf(), devicePtr->_defaultDepthView.Get());
    }

    void D3D11RenderSystem::_updateRenderStateBeforeRendering()
    {
        if (_rasterizerDescChagned)
        {
            _rasterizerDescChagned = false;

            D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);
            HRESULT hr = devicePtr->_device->CreateRasterizerState(&_rasterizer, _rasterizerState.ReleaseAndGetAddressOf());
            if (FAILED(hr))
            {
                GHOST_LOG_FORMAT_ERROR("Failed to create rasterizer state.");
            }

            devicePtr->_context->RSSetState(_rasterizerState.Get());
        }

        if (_depthStencilDescChanged)
        {
            _depthStencilDescChanged = false;

            D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);
            HRESULT hr = devicePtr->_device->CreateDepthStencilState(&_depthStencilDesc, _depthStencilState.ReleaseAndGetAddressOf());
            if (FAILED(hr))
            {
                GHOST_LOG_FORMAT_ERROR("Failed to create depthstencil state.");
            }

            devicePtr->_context->OMSetDepthStencilState(_depthStencilState.Get(), 0);
        }

        if (_blendDescChanged)
        {
            _blendDescChanged = false;

            D3D11RenderDevicePtr devicePtr = GHOST_SMARTPOINTER_CAST(D3D11RenderDevice, _renderDevice);
            HRESULT hr = devicePtr->_device->CreateBlendState(&_blendDesc, _blendState.ReleaseAndGetAddressOf());
            if (FAILED(hr))
            {
                GHOST_LOG_FORMAT_ERROR("Failed to create blend state.");
            }

            devicePtr->_context->OMSetBlendState(_blendState.Get(), 0, 0xffffffff);
        }
    }
}