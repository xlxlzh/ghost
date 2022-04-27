#include "GhostConfig.h"
#include "DataStream.h"
#include <vector>
#include "LogManager.h"

#include "../../External/tinyxml2/tinyxml2.h"

namespace ghost
{
    RendersystemType GetRenderTypeByString(const char* str)
    {
        if (strcmp(str, "D3D11") == 0)
            return RENDER_D3D11;
        else if (strcmp(str, "D3D12") == 0)
            return RENDER_D3D12;
        else if (strcmp(str, "VULKAN") == 0)
            return RENDER_VULKAN;
        else
            return RENDER_VULKAN;  //Because vulkan can be used mult-platform.
    }

    MSAA GetMSAATypeByString(const char* str)
    {
        if (strcmp(str, "1x") == 0)
            return _1x;
        else if (strcmp(str, "2x") == 0)
            return _2x;
        else if (strcmp(str, "4x") == 0)
            return _4x;
        else if (strcmp(str, "8x") == 0)
            return _8x;
        else if (strcmp(str, "16x") == 0)
            return _16x;
        else
            return _4x;
    }

    PlatformType GetPlatformByString(const char* str)
    {
        if (strcmp(str, "WIN32") == 0)
            return PLATFORM_WIN32;
        else if (strcmp(str, "LINUX") == 0)
            return PLATFORM_LINUX;
        else
            return PLATFORM_WIN32;
    }

    void GhostConfig::_initDefault()
    {
        _default = true;

        _globalConfig._fullscreen = false;
        _globalConfig._logFile = "GhostLogFile.log";
        _globalConfig._platform = PLATFORM_WIN32;
        _globalConfig._renderType = RENDER_D3D11;
        _globalConfig._width = 800;
        _globalConfig._height = 600;

        _renderConfig._msaa = _4x;
        _renderConfig._shadowResolution = 2048;
    }

    bool GhostConfig::loadConfig(const std::string& configFile)
    {
        _initDefault();

        DataStreamPtr data = GHOST_MAKE_SMART_POINTER(FileStream, configFile);
        if (!data || !data->isOpened())
            return false;

        std::vector<char> bytes;

        unsigned fileSize = data->getSize();
        bytes.resize(fileSize);

        fileSize = data->read(&bytes[0], fileSize);
        data->close();

        if (fileSize <= 0)
            return false;

        tinyxml2::XMLDocument doc;
        doc.Parse(&bytes[0], fileSize);

        tinyxml2::XMLElement* root = doc.RootElement();
        if (strcmp(root->Name(), "GhostConfig"))
        {
            GHOST_LOG_FORMAT_ERROR("%s: config file parse error.", __FUNCTION__);
            return false;
        }

        // Global settings
        tinyxml2::XMLElement* globalConfig = root->FirstChildElement("Global");
        if (!globalConfig)
        {
            GHOST_LOG_FORMAT_ERROR("%s: config file parse error at <Global>.", __FUNCTION__);
            return false;
        }

        tinyxml2::XMLElement* platform = globalConfig->FirstChildElement("Platform");
        if (!platform)
        {
            GHOST_LOG_FORMAT_ERROR("%s: config file parse error at <Global/Platform>.", __FUNCTION__);
            return false;
        }
        _globalConfig._platform = GetPlatformByString(platform->Attribute("value"));

        tinyxml2::XMLElement* fullscreen = globalConfig->FirstChildElement("Fullscreen");
        if (!fullscreen)
        {
            GHOST_LOG_FORMAT_ERROR("%s: config file parse error at <Global/Fullscreen>.", __FUNCTION__);
            return false;
        }
        _globalConfig._fullscreen = fullscreen->BoolAttribute("value");

        tinyxml2::XMLElement* res = globalConfig->FirstChildElement("Resolution");
        if (!res)
        {
            GHOST_LOG_FORMAT_ERROR("%s: config file parse error at <Global/Resolution>.", __FUNCTION__);
            return false;
        }
        _globalConfig._width = res->UnsignedAttribute("width");
        _globalConfig._height = res->UnsignedAttribute("height");

        tinyxml2::XMLElement* logEle = globalConfig->FirstChildElement("LogFile");
        if (!logEle)
        {
            GHOST_LOG_FORMAT_ERROR("%s: config file parse error at <Global/LogFile>.", __FUNCTION__);
            return false;
        }
        _globalConfig._logFile = logEle->Attribute("value");

        tinyxml2::XMLElement* renderType = globalConfig->FirstChildElement("RenderType");
        if (!renderType)
        {
            GHOST_LOG_FORMAT_ERROR("%s: config file parse error at <Global/RenderType>.", __FUNCTION__);
            return false;
        }
        _globalConfig._renderType = GetRenderTypeByString(renderType->Attribute("value"));

        // Render settings
        tinyxml2::XMLElement* renderConfig = root->FirstChildElement("Render");
        if (!renderConfig)
        {
            GHOST_LOG_FORMAT_ERROR("%s: config file parse error at <Render>.", __FUNCTION__);
            return false;
        }

        tinyxml2::XMLElement* msaa = renderConfig->FirstChildElement("MSAA");
        if (!msaa)
        {
            GHOST_LOG_FORMAT_ERROR("%s: config file parse error at <Render/MSAA>.", __FUNCTION__);
            return false;
        }
        _renderConfig._msaa = GetMSAATypeByString(msaa->Attribute("value"));

        tinyxml2::XMLElement* shadowResolution = renderConfig->FirstChildElement("ShadowResolution");
        if (!shadowResolution)
        {
            GHOST_LOG_FORMAT_ERROR("%s: config file parse error at <Render/ShadowResolution>.", __FUNCTION__);
            return false;
        }
        _renderConfig._shadowResolution = shadowResolution->IntAttribute("value");

        return true;
    }

    bool GhostConfig::saveConfigTo(const std::string& configFile)
    {
        return true;
    }
}