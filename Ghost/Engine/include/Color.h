#ifndef _COLOR_H_
#define _COLOR_H_

namespace ghost
{
    class Color
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