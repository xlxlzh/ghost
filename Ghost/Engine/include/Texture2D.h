#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_

#include "Resource.h"

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

    protected:
        unsigned _width;
        unsigned _height;
    };

    DECLAR_SMART_POINTER(Texture2D)
}

#endif