#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

#include "Resource.h"
#include "Color.h"

namespace ghost
{
    enum TextureUsage
    {
        TEX_USAGE_SHADERRESOURCE,
        TEX_USAGE_RENDERTARGET,
        TEX_USAGE_DEPTH,
        TEX_USAGE_SHADOWMAP,
    };

    class GHOST_API Texture2D : public Resource
    {
    public:
        Texture2D();
        Texture2D(const std::string& name, int flag);
        ~Texture2D();
        int getWidth() const { return _width; }
        int getHeight() const { return _height; }

        virtual bool load(DataStream& dataStream) override;

        static int getTypeStatic() { return RESOURCE_TEXTURE2D; }

    protected:
        void _setData(const unsigned char* data);
        virtual void _createTextureInternal() { }

    protected:
        int _width;
        int _height;
        int _components;

        bool _mipmap;
        int _mipmapLevel;

        GhostColorFormat _format = GHOST_FORMAT_UNKNOWN;
        unsigned char* _datas = nullptr;
        int _dataSize = 0;

        BufferUsage _usage = USAGE_STATIC;
    };

    DECLAR_SMART_POINTER(Texture2D)
}

#endif