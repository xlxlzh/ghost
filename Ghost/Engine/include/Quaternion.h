#ifndef _QUATERNION_H_
#define _QUATERNION_H_

namespace ghost
{
    template <typename T>
    class Quaternion
    {
    public:
        Quaternion() : _x(0.0), _y(0.0), _z(0.0), _w(0.0) { }
        Quaternion(T x, T y, T z, T w) £º _x(x), _y(y), _z(z), _w(w) { }

        Quaternion<T> operator* (const Quaternion<T>& q) const
        {
            return Quaternion<T>(
                _y * q._z - _z * q._y + q._x * _w + _x * q._w,
                _z * q._x - _x * q._z + q._y * _w + _y * q._w,
                _x * q._y - _y * q._x + q._z * _w + _z * q._w,
                _w * q._w - (_x * q._x + _y * q._y + _z * q._z)
                );
        }

        Quaternion<T>& operator*= (const Quaternion<T>& q)
        {
            *this = *this * q;
            return *this;
        }

    public:
        T _x, _y, _z, _w;
    };

    using Quaternionf = Quaternion<float>;
}

#endif
