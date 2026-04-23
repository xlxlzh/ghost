#ifndef _D3D11_RENDERSYSTEM_H_
#define _D3D11_RENDERSYSTEM_H_

#include <functional>
#include <d3d11.h>
#include "RenderSystem.h"
#include "D3D11RenderDevice.h"
#include "D3D11Texture2D.h"

namespace ghost
{
    class GHOST_API D3D11RenderSystem final : public RenderSystem
    {
    public:
        D3D11RenderSystem(RenderDevicePtr device);
        ~D3D11RenderSystem();
        
        virtual bool InitRenderSystem() override;

        virtual void SetRenderTarget(RenderTargetPtr rt) override;
        virtual void ClearRenderTarget(TargetClear clearFlag = CLEAR_ALL, Color col = Color::Black, float z = 1.0, unsigned stencil = 0.0) override;

        virtual void SetVertexBuffer(VertexBufferPtr vBuffer) override;
        virtual void SetVertexBufferBinding(VertexBufferBindingPtr binding) override;
        virtual void SetIndexBuffer(IndexBufferPtr iBuffer) override;
        virtual void SetVertexDeclaration(VertexDeclarationPtr vDecl) override;
        virtual void SetConstBuffer(ShaderType shaderType, ConstBufferPtr constBuffer) override;

        virtual void SetPrimitiveType(PrimitiveType pType) override;

        virtual void SetShader(const Shader* shader) override;
        virtual void DrawPrimitive(unsigned numVertices, unsigned startIndex) override;
        virtual void DrawPrimitiveIndexed(unsigned numIndices, unsigned indexLocation, int baseVertIndex) override;
        virtual void DrawPrimitiveInstance() override;

        virtual void UseDefaultRenderTarget() override;

        virtual void BeginScene() override;
        virtual void EndScene() override;

        virtual void SetCullMode(CullMode cull) override;
        virtual void SetFillMode(FillMode fillMode) override;
        virtual void SetDepthBufferParams(bool depthTest, bool depthWrite, CompareFunction depthFunction) override;
        virtual void SetDepthTestEnable(bool enable) override;
        virtual void SetDepthWriteEnable(bool enable) override;
        virtual void SetDepthFunction(CompareFunction fun) override;
        virtual void SetColorBufferEnable(bool r, bool g, bool b, bool a) override;

        virtual void SetSamplerState(unsigned slot, const Sampler& sampler) override;
        virtual void SetTextureAddressingMode(unsigned slot, const Sampler::UVWAddressingMode& uvwMode) override;
        virtual void SetTexture(unsigned slot, Texture2DPtr tex2D) override;

        virtual void PushGPUEvent(const std::wstring& name) override;
        virtual void PopGPUEvent() override;

        virtual void Render(const RenderOperation& op) override;

    protected:
        void InternalClearRenderTarget(Color cl);
        void UpdateRenderStateBeforeRendering();

    protected:
        std::map<InputSignatureList*, ID3D11InputLayoutPtr> _inputlayouts;

        bool _rasterizerDescChagned = true;
        ID3D11RasterizerStatePtr _rasterizerState = nullptr;
#ifdef GHOST_USE_D3D_11_1
        D3D11_RASTERIZER_DESC1 _rasterizer;
#else
        D3D11_RASTERIZER_DESC _rasterizer;
#endif // GHOST_USE_D3D_11_1

        bool _depthStencilDescChanged = true;
        D3D11_DEPTH_STENCIL_DESC _depthStencilDesc;
        ID3D11DepthStencilStatePtr _depthStencilState = nullptr;

        bool _blendDescChanged = false;
        D3D11_BLEND_DESC _blendDesc;
        ID3D11BlendStatePtr _blendState = nullptr;

        D3D11_SAMPLER_DESC _samplerDesc;
        ID3D11SamplerStatePtr _samplerState = nullptr;

        D3D11Texture2DPtr _textures2DUnits[GHOST_MAX_TEXTURE_UNITS];

        using SetSRVFunction = std::function<void(unsigned, ID3D11ShaderResourceView *const *)>;
        SetSRVFunction _srvFunctionTable[SHADER_TYPE_NUM];

        using SetSamplerFunction = std::function<void(unsigned, ID3D11SamplerState *const *)>;
        SetSamplerFunction _samplerFunctionTable[SHADER_TYPE_NUM];

        FilterOptions _minFilters[GHOST_MAX_TEXTURE_UNITS];
        FilterOptions _magFilters[GHOST_MAX_TEXTURE_UNITS];
        FilterOptions _mipFilters[GHOST_MAX_TEXTURE_UNITS];

        struct D3DTextureStageDesc
        {
            ID3D11ShaderResourceView*  _tex;
            D3D11_SAMPLER_DESC  _samplerDesc;
            bool _used;
        } _texStageDesc[GHOST_MAX_TEXTURE_UNITS];

        unsigned   _lastTextureUnitState{0};
        bool       _samplerStatesChanged{false};
    };
}

#endif
