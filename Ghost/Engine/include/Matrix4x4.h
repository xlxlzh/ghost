#ifndef _mRIX4X4_H_
#define _mRIX4X4_H_

namespace ghost
{
	template<typename T>
	class Matrix4x4
	{
	public:
		Matrix4x4()
		{
			identify();
		}

		Matrix4x4(const T* floatArray)
		{
			memcpy(&_m[0], floatArray, sizeof(T) * 16);
		}

		Matrix4x4(T _1_1, T _1_2, T _1_3, T _1_4,
			T _2_1, T _2_2, T _2_3, T _2_4,
			T _3_1, T _3_2, T _3_3, T _3_4,
			T _4_1, T _4_2, T _4_3, T _4_4) :
			_11(_1_1), _12(_1_2), _13(_1_3), _14(_1_4),
			_21(_2_1), _22(_2_2), _23(_2_3), _24(_2_4),
			_31(_3_1), _32(_3_2), _33(_3_3), _34(_3_4),
			_41(_4_1), _42(_4_2), _43(_4_3), _44(_4_4)
		{

		}

		Matrix4x4(const Matrix4x4<T>& rhs)
		{
			_11 = rhs._11, _12 = rhs._12, _13 = rhs._13, _14 = rhs._14;
			_21 = rhs._21, _22 = rhs._22, _23 = rhs._23, _24 = rhs._24;
			_31 = rhs._31, _32 = rhs._32, _33 = rhs._33, _34 = rhs._34;
			_41 = rhs._41, _42 = rhs._42, _43 = rhs._43, _44 = rhs._44;
		}

		Matrix4x4<T>& operator= (const Matrix4x4<T>& rhs)
		{
			_11 = rhs._11, _12 = rhs._12, _13 = rhs._13, _14 = rhs._14;
			_21 = rhs._21, _22 = rhs._22, _23 = rhs._23, _24 = rhs._24;
			_31 = rhs._31, _32 = rhs._32, _33 = rhs._33, _34 = rhs._34;
			_41 = rhs._41, _42 = rhs._42, _43 = rhs._43, _44 = rhs._44;

			return *this;
		}

		Matrix4x4<T> operator- () const
		{
			Matrix4x4<T> m;
			m._11 = -_11, m._12 = -_12, m._13 = -_13, m._14 = -_14;
			m._21 = -_21, m._22 = -_22, m._23 = -_23, m._24 = -_24;
			m._31 = -_31, m._32 = -_32, m._33 = -_33, m._34 = -_34;
			m._41 = -_41, m._42 = -_42, m._43 = -_43, m._44 = -_44;

			return m;
		}

		Matrix4x4<T> operator+ (const Matrix4x4<T>& rhs) const
		{
			Matrix4x4<T> m;
			for (int i = 0; i < 16; ++i)
			{
				m._m[i] = this->_m[i] + rhs._m[i];
			}

			return m;
		}

		Matrix4x4<T>& operator+= (const Matrix4x4<T>& rhs)
		{
			for (int i = 0; i < 16; ++i)
			{
				_m[i] += rhs._m[i];
			}

			return *this;
		}

		Matrix4x4<T> operator* (const Matrix4x4<T>& rhs) const
		{
			Matrix4x4<T> m;

			m._11 = _11 * rhs._11 + _12 * rhs._21 + _13 * rhs._31 + _14 * rhs._41;
			m._12 = _11 * rhs._12 + _12 * rhs._22 + _13 * rhs._32 + _14 * rhs._42;
			m._13 = _11 * rhs._13 + _12 * rhs._23 + _13 * rhs._33 + _14 * rhs._43;
			m._14 = _11 * rhs._14 + _12 * rhs._24 + _13 * rhs._24 + _14 * rhs._44;

			m._21 = _21 * rhs._11 + _22 * rhs._21 + _23 * rhs._31 + _24 * rhs._41;
			m._22 = _21 * rhs._12 + _22 * rhs._22 + _23 * rhs._32 + _24 * rhs._42;
			m._23 = _21 * rhs._13 + _22 * rhs._23 + _23 * rhs._33 + _24 * rhs._43;
			m._24 = _21 * rhs._14 + _22 * rhs._24 + _23 * rhs._34 + _24 * rhs._44;

			m._31 = _31 * rhs._11 + _32 * rhs._21 + _33 * rhs._31 + _34 * rhs._41;
			m._32 = _31 * rhs._12 + _32 * rhs._22 + _33 * rhs._32 + _34 * rhs._42;
			m._33 = _31 * rhs._13 + _32 * rhs._23 + _33 * rhs._33 + _34 * rhs._43;
			m._34 = _31 * rhs._14 + _32 * rhs._24 + _33 * rhs._34 + _34 * rhs._44;

			m._41 = _41 * rhs._11 + _42 * rhs._21 + _43 * rhs._31 + _44 * rhs._41;
			m._42 = _41 * rhs._12 + _42 * rhs._22 + _43 * rhs._32 + _44 * rhs._42;
			m._43 = _41 * rhs._13 + _42 * rhs._23 + _43 * rhs._33 + _44 * rhs._43;
			m._44 = _41 * rhs._14 + _42 * rhs._24 + _43 * rhs._34 + _44 * rhs._44;

			return m;
		}

		Matrix4x4<T>& operator*= (const Matrix4x4<T>& rhs)
		{
			_11 = _11 * rhs._11 + _12 * rhs._21 + _13 * rhs._31 + _14 * rhs._41;
			_12 = _11 * rhs._12 + _12 * rhs._22 + _13 * rhs._32 + _14 * rhs._42;
			_13 = _11 * rhs._13 + _12 * rhs._23 + _13 * rhs._33 + _14 * rhs._43;
			_14 = _11 * rhs._14 + _12 * rhs._24 + _13 * rhs._24 + _14 * rhs._44;

			_21 = _21 * rhs._11 + _22 * rhs._21 + _23 * rhs._31 + _24 * rhs._41;
			_22 = _21 * rhs._12 + _22 * rhs._22 + _23 * rhs._32 + _24 * rhs._42;
			_23 = _21 * rhs._13 + _22 * rhs._23 + _23 * rhs._33 + _24 * rhs._43;
			_24 = _21 * rhs._14 + _22 * rhs._24 + _23 * rhs._34 + _24 * rhs._44;

			_31 = _31 * rhs._11 + _32 * rhs._21 + _33 * rhs._31 + _34 * rhs._41;
			_32 = _31 * rhs._12 + _32 * rhs._22 + _33 * rhs._32 + _34 * rhs._42;
			_33 = _31 * rhs._13 + _32 * rhs._23 + _33 * rhs._33 + _34 * rhs._43;
			_34 = _31 * rhs._14 + _32 * rhs._24 + _33 * rhs._34 + _34 * rhs._44;

			_41 = _41 * rhs._11 + _42 * rhs._21 + _43 * rhs._31 + _44 * rhs._41;
			_42 = _41 * rhs._12 + _42 * rhs._22 + _43 * rhs._32 + _44 * rhs._42;
			_43 = _41 * rhs._13 + _42 * rhs._23 + _43 * rhs._33 + _44 * rhs._43;
			_44 = _41 * rhs._14 + _42 * rhs._24 + _43 * rhs._34 + _44 * rhs._44;

			return *this;
		}

		Matrix4x4<T> operator* (const T& rhs) const
		{
			Matrix4x4<T> m;

			for (int i = 0; i < 16; ++i)
			{
				m._m[i] = _m[i] * rhs;
			}

			return m;
		}

		Matrix4x4<T>& operator*= (const T& rhs)
		{
			for (int i = 0; i < 16; ++i)
			{
				_m[i] *= rhs;
			}

			return *this;
		}

		Matrix4x4<T> transpose() const
		{
			Matrix4x4<T> m(*this);

			for (int y = 0; y < 4; ++y)
			{
				for (int x = 0; x < 4; ++x)
				{
					T tmp = m._m[y * 4 + x];
					m._m[y * 4 + x] = m._m[x * 4 + y];
					m._m[x * 4 + y] = tmp;
				}
			}

			return m;
		}

		Matrix4x4<T> inverse() const
		{
			Matrix4x4<T> m;

			T d = determinant();
			if (MathUtilities::equal<T>(d, 0))
			{
				return m;
			}
		}

		T determinant() const
		{
			//TODO
			return 0;
		}

		void identify()
		{
			_11 = 1, _12 = 0, _13 = 0, _14 = 0;
			_21 = 0, _22 = 1, _23 = 0, _24 = 0;
			_31 = 0, _32 = 0, _33 = 1, _34 = 0;
			_41 = 0, _42 = 0, _43 = 0, _44 = 1;
		}

	public:
		union
		{
			struct 
			{
				T _11, _12, _13, _14;
				T _21, _22, _23, _24;
				T _31, _32, _33, _34;
				T _41, _42, _43, _44;
			};

			T _m[16];
		};
	};

	using Matrix4x4f = Matrix4x4<float>;
}

#endif
