#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "MathUtilities.h"
#include "Matrix4x4.h"

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
		Vector3(T *const arr) : _x(arr[0]), _y(arr[1]), _z(arr[2]) { }

		Vector3(const Vector3<T>& rhs) : _x(rhs._x), _y(rhs._y), _z(rhs._z) { }

		Vector3<T>& operator= (const Vector3<T>& rhs) { _x = rhs._x; _y = rhs._y; _z = rhs._z; return *this; }

		bool operator== (const Vector3<T>& rhs) const { return this->equal(rhs); }
		bool operator!= (const Vector3<T>& rhs) const { return !this->equal(rhs); }

		Vector3<T>& operator+= (const Vector3<T>& rhs) { _x += rhs._x; _y += rhs._y; _z += rhs._z; return *this; }
		Vector3<T>& operator-= (const Vector3<T>& rhs) { _x -= rhs._x; _y -= rhs._y; _z -= rhs._z; return *this; }
		Vector3<T>& operator*= (const Vector3<T>& rhs) { _x *= rhs._x; _y *= rhs._y; _z *= rhs._z; return *this; }
		Vector3<T>& operator/= (const Vector3<T>& rhs) { _x /= rhs._x; _y /= rhs._y; _z /= rhs._z; return *this; }

		Vector3<T> operator+ (const Vector3<T>& rhs) const { return Vector3<T>(_x + rhs._x, _y + rhs._y, _z + rhs._z); }
		Vector3<T> operator- (const Vector3<T>& rhs) const { return Vector3<T>(_x - rhs._x, _y - rhs._y, _z - rhs._z); }
		Vector3<T> operator* (const Vector3<T>& rhs) const { return Vector3<T>(_x * rhs._x, _y * rhs._y, _z * rhs._z); }
		Vector3<T> operator/ (const Vector3<T>& rhs) const { return Vector3<T>(_x / rhs._x, _y / rhs._y, _z / rhs._z); }

		Vector3<T> operator* (const T& rhs) const { return Vector3<T>(rhs * _x, rhs * _y, rhs * _z); }
		Vector3<T>& operator*= (const T& rhs) { _x *= rhs; _y *= rhs; _z *= rhs; return *this; }
		Vector3<T> operator/ (const T& rhs) const { return Vector3<T>(_x / rhs, _y / rhs, _z / rhs); }
		Vector3<T>& operator/= (const T& rhs) { _x /= rhs; _y /= rhs; _z /= rhs; return *this; }

		Vector3<T> operator- () const { return Vector3<T>(-_x, -_y, -_z); }

        Vector3<T> operator* (const Matrix4x4<T>& rhs) const
        {
            return Vector3<T>(_x * rhs._11 + _y * rhs._21 + _z * rhs._31 + rhs._41,
                _x * rhs._12 + _y * rhs._22 + _z * rhs._32 + rhs._42,
                _x * rhs._13 + _y * rhs._23 + _z * rhs._33 + rhs._43
                );
        }

		T dotProduct(const Vector3<T>& rhs) const { return _x * rhs._x + _y * rhs._y + _z * rhs._z; }

		bool equal(const Vector3<T>& rhs) const
		{
			return MathUtilities::equal(_x, rhs._x) && MathUtilities::equal(_y, rhs._y) && MathUtilities::equal(_z, rhs._z);
		}

		T length() const
		{
			return MathUtilities::sqrt<T>(_x * _x + _y * _y + _z * _z);
		}

		Vector3<T> crossProduct(const Vector3<T>& rhs) const
		{
			return Vector3<T>(_y * rhs._z - _z * rhs._y, _z * rhs._x - _x * rhs._z, _x * rhs._y - _y * rhs._x);
		}

		Vector3<T> normalize()
		{
			T vecLength = length();
			assert(vecLength > 0.0f);

			_x /= vecLength;
			_y /= vecLength;
			_z /= vecLength;

            return *this;
		}

	public:
		T _x, _y, _z;
	};

	template<typename T>
	Vector3<T> operator* (const T& lhs, const Vector3<T>& rhs)
	{
		return rhs * lhs;
	}

	using Vector3f = Vector3<float>;
}

#endif
