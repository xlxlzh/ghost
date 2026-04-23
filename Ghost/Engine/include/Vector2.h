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
		Vector2(T* const arr) : _x(arr[0]), _y(arr[1]) { }

		Vector2(const Vector2<T>& rhs) : _x(rhs._x), _y(rhs._y) { }

		Vector2<T>& operator= (const Vector2<T>& rhs) { _x = rhs._x; _y = rhs._y; return *this; }

		bool operator== (const Vector2<T>& rhs) const
		{
			return this->Equal(rhs);
		}

		bool operator!= (const Vector2<T>& rhs) const
		{
			return !this->Equal(rhs);
		}

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

		Vector2<T> operator- () const { return Vector2<T>(-_x, -_y); }

		T DotProduct(const Vector2<T>& rhs) const { return _x * rhs._x + _y * rhs._y; }

		T Length() const { return Sqrt(_x * _x + _y * _y); }

		bool Equal(const Vector2<T>& rhs) const
		{
			return MathUtilities::Equal(_x, rhs._x) && MathUtilities::Equal(_y, rhs._y);
		}

		void Normalize()
		{
			T vecLength = length();
			assert(vecLength > 0.0f);

			_x /= vecLength;
			_y /= vecLength;
		}

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
