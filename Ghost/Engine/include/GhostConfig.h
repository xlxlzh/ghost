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
        bool loadConfig(const std::string& configFile);
        bool saveConfigTo(const std::string& configFile);

        const GlobalConfig& getGlobalConfig() { return _globalConfig; }
        const RenderConfig& getRenderConfig() { return _renderConfig; }

        bool isDefault() const { return _default; }

    private:
        void _initDefault();

    private:
        GlobalConfig _globalConfig;
        RenderConfig _renderConfig;

        std::string _configFile;

        bool _default{ true };
    };
}

#endif // !_GHOST_CONFIG_H_
