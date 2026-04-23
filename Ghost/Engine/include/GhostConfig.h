#ifndef _GHOST_CONFIG_H_
#define _GHOST_CONFIG_H_

#include <string>
#include "SingleTon.h"
#include "RenderCommon.h"
#include "Common.h"

namespace ghost
{
    struct  GHOST_API GlobalConfig
    {
        std::string _logFile;
        RendersystemType _renderType;
        PlatformType _platform;
        bool _fullscreen;
        unsigned _width, _height;
    };

    struct GHOST_API RenderConfig
    {
        MSAA _msaa;
        unsigned _shadowResolution;
    };

    class GHOST_API GhostConfig : public SingleTon<GhostConfig>
    {
    public:
        bool LoadConfig(const std::string& configFile);
        bool SaveConfigTo(const std::string& configFile);

        const GlobalConfig& GetGlobalConfig() { return _globalConfig; }
        const RenderConfig& GetRenderConfig() { return _renderConfig; }

        bool IsDefault() const { return _default; }

    private:
        void InitDefault();

    private:
        GlobalConfig _globalConfig;
        RenderConfig _renderConfig;

        std::string _configFile;

        bool _default{ true };
    };
}

#endif // !_GHOST_CONFIG_H_
