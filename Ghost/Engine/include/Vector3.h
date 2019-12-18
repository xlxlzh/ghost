#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "MathUtilities.h"

namespace ghost
{
	template<typename T>
	class Vector3
	{
	public:
		Vector3() = default;
		Vector3(T x, T y, T z) : _x(x), _y(y), _z(z) { }
		Vector3(T rhs) : _x(rhs), _y(rhs), _z(rhs) { }
		Vector3(const T* arr) : _x(arr[0]), _y(arr[1]), _z(arr[2]) { }
		Vector3(T const* arr) : _x(arr[0]), _y(arr[1]), _z(arr[2]) { }

		Vector3(const Vector3<T>& rhs) : _x(rhs._x), _y(rhs._y), _z(rhs._z) { }

		Vector3<T>& operator= (const Vector3<T>& rhs) { _x = rhs._x; _y = rhs._y; _z = rhs._z; return *this; }

		Vector3<T>& operator+= (const Vector3<T>& rhs) { _x += rhs._x; _y += rhs._y; _z += rhs._z; return *this; }
		Vector3<T>& operator+= (const Vector3<T>& rhs) { _x += rhs._x; _y += rhs._y; _z += rhs._z; return *this; }
		Vector3<T>& operator*= (const Vector3<T>& rhs) { _x *= rhs._x; _y *= rhs._y; _z *= rhs._z; return *this; }
		Vector3<T>& operator/= (const Vector3<T>& rhs) { _x /= rhs._x; _y /= rhs._y; _z /= rhs._z; return *this; }

		Vector3<T> operator+ (const Vector3<T>& rhs) const { return Vector3<T>(_x + rhs._x, _y + rhs._y, _z + rhs._z); }
		Vector3<T> operator- (const Vector3<T>& rhs) const { return Vector3<T>(_x - rhs._x, _y - rhs._y, _z - rhs._z); }
		Vector3<T> operator* (const Vector3<T>& rhs) const { return Vector3<T>(_x * rhs._x, _y * rhs._y, _z * rhs._z); }
		Vector3<T> operator/ (const Vector3<T>& rhs) const { return Vector3<T>(_x / rhs._x, _y / rhs._y, _z / rhs._z); }

		Vector3<T> operator* (const T& rhs) const { return Vector3<T>(rhs * _x, rhs * _y, rhs * _z); }
		Vector3<T>& operator*= (const T& rhs) { _x *= rhs; _y *= rhs; _z *= rhs; return *this; }
		Vector3<T> operator/ (const T& rhs) const { return Vector2<T>(__x / rhs, _y / rhs, _z / rhs); }
		Vector3<T>& operator/= (const T& rhs) { _x /= rhs; _y /= rhs; _z /= rhs; return *this; }

		T dot(const Vector3<T>& rhs) const { return _x * rhs._x + _y * rhs._y + _z * rhs._z; }

		T length() const
		{
			return MathUtilities::sqrt<T>(_x * _x + _y * _y + _z * _z);
		}

	public:
		T _x, _y, _z;
	};

	using Vector3f = Vector3<float>;
}

#endif
