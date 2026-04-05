#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include "MathUtilities.h"
#include "Matrix4x4.h"

namespace ghost
{
	template<typename T>
	class Vector4
	{
	public:
		Vector4() = default;
		Vector4(T x, T y, T z, T w) : _x(x), _y(y), _z(z), _w(w) { }
		Vector4(T rhs) : _x(rhs), _y(rhs), _z(rhs), _w(rhs) { }
        Vector4(const Vector3<T>& rhs) : _x(rhs._x), _y(rhs._y), _z(rhs._z), _w(1.0) { }
		Vector4(const T* arr) : _x(arr[0]), _y(arr[1]), _z(arr[2]), _w(arr[3]) { }
		Vector4(T* const arr) : _x(arr[0]), _y(arr[1]), _z(arr[2]), _w(arr[3]) { }

		Vector4(const Vector4<T>& rhs) : _x(rhs._x), _y(rhs._y), _z(rhs._z), _w(rhs._w) { }

		Vector4<T>& operator= (const Vector4<T>& rhs) { _x = rhs._x; _y = rhs._y; _z = rhs._z; _w = rhs._w; return *this; }

		bool operator== (const Vector4<T>& rhs) const { return this->equal(rhs); }
		bool operator!= (const Vector4<T>& rhs) const { return !this->equal(rhs); }

		Vector4<T> operator* (const T& rhs) const { return Vector4<T>(rhs * _x, rhs * _y, rhs * _z, rhs * _w); }
		Vector4<T>& operator*= (const T& rhs) { _x *= rhs; _y *= rhs; _z *= rhs; _w *= rhs; return *this; }
		Vector4<T> operator/ (const T& rhs) const { return Vector4<T>(_x / rhs, _y / rhs, _z / rhs, _w / rhs); }
		Vector4<T>& operator/= (const T& rhs) { _x /= rhs; _y /= rhs; _z /= rhs; _w /= rhs; return *this; }

		Vector4<T> operator- () const { return Vector4<T>(-_x, -_y, -_z, -_w); }

        Vector4<T> operator* (const Matrix4x4<T>& rhs) const
        {
            return Vector4<T>(_x * rhs._11 + _y * rhs._21 + _z * rhs._31 + _w * rhs._41,
                _x * rhs._12 + _y * rhs._22 + _z * rhs._32 + _w * rhs._42,
                _x * rhs._13 + _y * rhs._23 + _z * rhs._33 + _w * rhs._43,
                _x * rhs._14 + _y * rhs._24 + _z * rhs._34 + _w * rhs._44);
        }

		bool equal(const Vector4<T>& rhs) const
		{
			return MathUtilities::equal(_x, rhs._x) && MathUtilities::equal(_y, rhs._y) && MathUtilities::equal(_z, rhs._z) && MathUtilities::equal(_w, rhs._w);
		}

	public:
		T _x, _y, _z, _w;
	};

	using Vector4f = Vector4<float>;
}

#endif
