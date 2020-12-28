#ifndef _QUATERNION_H_
#define _QUATERNION_H_

namespace ghost
{
    template <typename T>
    class Quaternion
    {
    public:
        Quaternion() : _x(0.0), _y(0.0), _z(0.0), _w(0.0) { }
        Quaternion(T x, T y, T z, T w) : _x(x), _y(y), _z(z), _w(w) { }

        Quaternion(const T eulerX, const T eulerY, const T eulerZ)
        {
            Quaternion<T> roll(0, 0, MathUtilities::sin<T>(eulerZ * 0.5), MathUtilities::cos<T>(eulerZ * 0.5));
            Quaternion<T> pitch(MathUtilities::sin<T>(eulerX * 0.5), 0, 0, MathUtilities::cos<T>(eulerX * 0.5));
            Quaternion<T> yaw(0, MathUtilities::sin<T>(eulerY * 0.5), 0, MathUtilities::cos<T>(eulerY * 0.5));

            *this = roll * pitch * yaw;
        }

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
