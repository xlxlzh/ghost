#ifndef _VECTOR2_H_
#define _VECTOR2_H_

namespace ghost
{
	template<typename T>
	class Vector2
	{
	public:
		Vector2() : _x(0), _y(0) { }
		Vector2(T x, T y) : _x(x), _y(y) { }
		Vector2(T rhs) : _x(rhs), _y(rhs) { }
		Vector2(const T* arr) : _x(arr[0]), _y(arr[1]) { }
		Vector2(T const* arr) : _x(arr[0]), _y(arr[1]) { }

		Vector2(const Vector2<T>& rhs) : _x(rhs._x), _y(rhs._y) { }

		Vector2<T>& operator= (const Vector2<T>& rhs) { _x = rhs._x; _y = rhs._y; return *this; }

		Vector2<T> operator+ (const Vector2<T>& rhs) const { return Vector2<T>(_x + rhs._x, _y + rhs._y); }
		Vector2<T> operator- (const Vector2<T>& rhs) const { return Vector2<T>(_x - rhs._x, _y - rhs._y); }
		Vector2<T> operator* (const Vector2<T>& rhs) const { return Vector2<T>(_x * rhs._x, _y * rhs._y); }
		Vector2<T> operator/ (const Vector2<T>& rhs) const { return Vector2<T>(_x / rhs._x, _y / rhs._y); }

		Vector2<T>& operator+= (const Vector2<T>& rhs) { _x += rhs._x; _y += rhs._y; return *this; }
		Vector2<T>& operator-= (const Vector2<T>& rhs) { _x -= rhs._x; _y -= rhs._y; return *this; }
		Vector2<T>& operator*= (const Vector2<T>& rhs) { _x *= rhs._x; _y *= rhs._y; return *this; }
		Vector2<T>& operator/= (const Vector2<T>& rhs) { _x /= rhs._x; _y /= rhs._y; return *this; }

		Vector2<T> operator* (const T& rhs) const { return Vector2<T>(rhs * _x, rhs * _y); }
		Vector2<T>& operator*= (const T& rhs) { _x *= rhs; _y *= rhs; return *this; }
		Vector2<T> operator/ (const T& rhs) const { return Vector2<T>(_x / rhs, _y / rhs); }
		Vector2<T>& operator/= (const T& rhs) { _x /= rhs; _y /= rhs; return *this; }

		T dot(const Vector2<T>& rhs) const { return _x * rhs._x + _y * rhs._y; }

		T length() const { return sqrt(_x * rhs._x + _y * rhs._y); }

	public:
		T _x, _y;
	};

	template<typename T>
	Vector2<T> operator* (const T& lhs, const Vector2<T>& rhs)
	{
		return rhs * lhs;
	}

	using Vector2f = Vector2<float>;
}

#endif
