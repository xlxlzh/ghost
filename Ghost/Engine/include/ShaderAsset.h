#ifndef _SHADER_ASSET_H_
#define _SHADER_ASSET_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "Resource.h"
#include "ShaderResource.h"
#include "RenderCommon.h"

namespace ghost
{
    enum RenderPass
    {
        RENDER_PASS_FORWARD,
        RENDER_PASS_SHADOW,
        RENDER_PASS_DEPTH,
        RENDER_PASS_DEFERRED,
        RENDER_PASS_NUM
    };

    struct GHOST_API ShaderEntryDefinition
    {
        ShaderType type = SHADER_TYPE_NUM;
        std::string entry;
    };

    struct GHOST_API PassRenderState
    {
        bool _overrideDepthTest = false;
        bool _depthTest = true;

        bool _overrideDepthWrite = false;
        bool _depthWrite = true;

        bool _overrideDepthFunc = false;
        CompareFunction _depthFunc = COMPARISON_LESS_EQUAL;

        bool _overrideCull = false;
        CullMode _cull = CULL_CW;
    };

    struct GHOST_API ShaderPassDefinition
    {
        RenderPass _passType = RENDER_PASS_FORWARD;
        std::string _sourcePath;
        std::vector<ShaderEntryDefinition> _entries;
        std::unordered_map<std::string, std::string> _defines;
        PassRenderState _renderState;
    };

    class GHOST_API ShaderAsset : public Resource
    {
    public:
        ShaderAsset();

        virtual bool Load(DataStream& dataStream) override;
        virtual bool Save(DataStream& dataStream) override;

        static int GetTypeStatic() { return RESOURCE_SHADER_ASSET; }

        const std::vector<ShaderPassDefinition>& GetPasses() const { return _passes; }

    private:
        std::vector<ShaderPassDefinition> _passes;
    };

    DECLAR_SMART_POINTER(ShaderAsset);
}

#endif
