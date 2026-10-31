#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

#include "Texture2DRes.h"
#include "Color.h"
#include "GPUResourceFlags.h"

namespace ghost
{
    class GHOST_API Texture2D
     {
    public:
        Texture2D();
        Texture2D(const Texture2DResPtr& texRes);
        Texture2D(const std::string& name);
        virtual ~Texture2D();
        
        int GetWidth() const { return _width; }
        int GetHeight() const { return _height; }
        PixelFormat GetFormat() const { return _format; }

    protected:
        virtual void InternalCreateTexture() { }

    protected:
        int _width = 0;
        int _height = 0;
        int _components = 0;
        Texture2DResPtr _rawTexture = nullptr;

        bool _mipmap = false;
        int _mipmapLevel = 1;
        
        std::string _name;
        
        PixelFormat _format = PixelFormat::PF_UNKNOWN;
        ResourceUsage _usage = USAGE_STATIC;
    };

    DECLAR_SMART_POINTER(Texture2D)
}

#endif