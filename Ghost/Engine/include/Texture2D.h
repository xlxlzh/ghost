#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

#include "Resource.h"
#include "Color.h"

namespace ghost
{
    class GHOST_API Texture2D : public Resource
    {
    public:
        Texture2D();
        Texture2D(const std::string& name, int flag);
        ~Texture2D();
        unsigned getWidth() const { return _width; }
        unsigned getHeight() const { return _height; }

        virtual bool load(DataStream& dataStream) override;

    protected:
        unsigned _width;
        unsigned _height;

        bool _mipmap;
        int _mipmapLevel;

        GhostColorFormat _format = GHOST_FORMAT_UNKNOWN;
        void* _datas = nullptr;
        unsigned _dataSize = 0;
    };

    DECLAR_SMART_POINTER(Texture2D)
}

#endif