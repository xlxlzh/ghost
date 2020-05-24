#include "Color.h"
#include "MathUtilities.h"

#include <cassert>

namespace ghost
{
    Color Color::Black = Color(0.0, 0.0, 0.0);
    Color Color::Red = Color(1.0, 0.0, 0.0);
    Color Color::Blue = Color(0.0, 0.0, 1.0);
    Color Color::Green = Color(0.0, 1.0, 0.0);

    bool Color::operator== (const Color& rhs) const
    {
        return MathUtilities::equal<float>(_r, rhs._r) &&
            MathUtilities::equal<float>(_g, rhs._g) &&
            MathUtilities::equal<float>(_b, rhs._b) &&
            MathUtilities::equal<float>(_a, rhs._a);
    }

    bool Color::operator!= (const Color& rhs) const
    {
        return !MathUtilities::equal<float>(_r, rhs._r) ||
            !MathUtilities::equal<float>(_g, rhs._g) ||
            !MathUtilities::equal<float>(_b, rhs._b) ||
            !MathUtilities::equal<float>(_a, rhs._a);
    }

    float Color::operator[] (const size_t i) const
    {
        assert(i < 4);
        return _color[i];
    }

    float& Color::operator[] (const size_t i)
    {
        assert(i < 4);
        return _color[i];
    }

    const float* Color::getColorPtr() const
    {
        return &_color[0];
    }

    Color Color::operator+ (const Color& rhs) const
    {
        Color cl;

        cl._r = _r + rhs._r;
        cl._g = _g + rhs._g;
        cl._b = _b + rhs._b;
        cl._a = _a + rhs._a;

        return cl;
    }

    Color Color::operator- (const Color& rhs) const
    {
        Color cl;

        cl._r = _r - rhs._r;
        cl._g = _g - rhs._g;
        cl._b = _b - rhs._b;
        cl._a = _a - rhs._a;

        return cl;
    }

    Color Color::operator* (const Color& rhs) const
    {
        Color cl;

        cl._r = _r * rhs._r;
        cl._g = _g * rhs._g;
        cl._b = _b * rhs._b;
        cl._a = _a * rhs._a;

        return cl;
    }

    Color Color::operator/ (const Color& rhs) const
    {
        Color cl;

        cl._r = _r / rhs._r;
        cl._g = _g / rhs._g;
        cl._b = _b / rhs._b;
        cl._a = _a / rhs._a;

        return cl;
    }

    Color Color::operator* (const float scale) const
    {
        Color cl;

        cl._r = _r * scale;
        cl._g = _g * scale;
        cl._b = _b * scale;
        cl._a = _a * scale;

        return cl;
    }

    Color Color::operator/ (const float scale) const
    {
        assert(scale != 0.0f);

        Color cl;

        float inv = 1.0f / scale;
        cl._r = _r * inv;
        cl._g = _g * inv;
        cl._b = _b * inv;
        cl._a = _a * inv;

        return cl;
    }

    Color& Color::operator+= (const Color& rhs)
    {
        _r += rhs._r;
        _g += rhs._g;
        _b += rhs._b;
        _a += rhs._a;

        return *this;
    }

    Color& Color::operator-= (const Color& rhs)
    {
        _r -= rhs._r;
        _g -= rhs._g;
        _b -= rhs._b;
        _a -= rhs._a;

        return *this;
    }

    Color& Color::operator*= (const Color& rhs)
    {
        _r *= rhs._r;
        _g *= rhs._g;
        _b *= rhs._b;
        _a *= rhs._a;

        return *this;
    }

    Color& Color::operator/= (const float scale)
    {
        assert(scale != 0.0f);

        float inv = 1.0f / scale;
        _r *= scale;
        _g *= scale;
        _b *= scale;
        _a *= scale;

        return *this;
    }
}