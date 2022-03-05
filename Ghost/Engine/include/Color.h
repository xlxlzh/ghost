#ifndef _COLOR_H_
#define _COLOR_H_

#include "Ghost.h"

namespace ghost
{
    enum GhostColorFormat
    {
        GHOST_FORMAT_UNKNOWN,

        GHOST_FORMAT_A8R8G8B8,
        GHOST_FORMAT_R8G8B8A8,
        GHOST_FORMAT_D24S8,

        GHOST_FORMAT_FLOAT_32,
        GHOST_FORMAT_FLOAT_16,

        GHOST_FORMAT_R32G32B32A32_FLOAT,
        GHOST_FORMAT_R32G32B32_FLOAT,
    };

    class GHOST_API FormatUtilies
    {
    public:
        static int getFormatSizeInByte(GhostColorFormat fmt)
        {
            switch (fmt)
            {
            case GHOST_FORMAT_A8R8G8B8:
            case GHOST_FORMAT_R8G8B8A8:
            case GHOST_FORMAT_D24S8:
            case GHOST_FORMAT_FLOAT_32:
                return 4;
            case GHOST_FORMAT_FLOAT_16:
                return 2;
            case GHOST_FORMAT_R32G32B32A32_FLOAT:
                return 16;
            case GHOST_FORMAT_R32G32B32_FLOAT:
                return 12;
            }

            return 0;
        }

        static int getFormatSizeInBits(GhostColorFormat fmt)
        {
            switch (fmt)
            {
            case GHOST_FORMAT_A8R8G8B8:
            case GHOST_FORMAT_R8G8B8A8:
            case GHOST_FORMAT_D24S8:
            case GHOST_FORMAT_FLOAT_32:
                return 32;
            case GHOST_FORMAT_FLOAT_16:
                return 16;
            case GHOST_FORMAT_R32G32B32A32_FLOAT:
                return 128;
            case GHOST_FORMAT_R32G32B32_FLOAT:
                return 96;
            }

            return 0;
        }
    };

    class GHOST_API Color
    {
    public:
        Color() = default;
        Color(float r, float g, float b, float a = 1.0f) : _r(r), _g(g), _b(b), _a(a) { }

        bool operator!= (const Color& rhs) const;
        bool operator== (const Color& rhs) const;

        float operator[] (const size_t i) const;
        float& operator[] (const size_t i);

        const float* getColorPtr() const;

        Color operator+ (const Color& rhs) const;
        Color operator- (const Color& rhs) const;
        Color operator* (const Color& rhs) const;
        Color operator/ (const Color& rhs) const;
        Color operator* (const float scale) const;
        Color operator/ (const float scale) const;

        Color& operator+= (const Color& rhs);
        Color& operator-= (const Color& rhs);
        Color& operator*= (const Color& rhs);
        Color& operator/= (const float scale);

        friend Color operator* (const float scale, const Color& rhs)
        {
            Color cl;

            cl._r = rhs._r * scale;
            cl._g = rhs._g * scale;
            cl._b = rhs._b * scale;
            cl._a = rhs._a * scale;

            return cl;
        }

    public:
        static Color Black;
        static Color Red;
        static Color Blue;
        static Color Green;
        static Color White;
        static Color Gray;

    public:
        union
        {
            struct
            {
                float _r, _g, _b, _a;
            };
            float _color[4];
        };
    };
}

#endif