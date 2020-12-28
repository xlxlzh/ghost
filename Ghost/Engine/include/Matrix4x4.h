#ifndef _mRIX4X4_H_
#define _mRIX4X4_H_

#include "Quaternion.h"

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

        Matrix4x4(const Quaternion<T>& q)
        {
            float x2 = q._x + q._x, y2 = q._y + q._y, z2 = q._z + q._z;
            float xx = q._x * x2, xy = q._x * y2, xz = q._x * z2;
            float yy = q._y * y2, yz = q._y * z2, zz = q._z * z2;
            float wx = q._w * x2, wy = q._w * y2, wz = q._w * z2;

            _11 = 1 - (yy + zz);
            _12 = xy - wz;
            _13 = xz + wy;
            _14 = 0;

            _21 = xy + wz;
            _22 = 1 - (xx + zz);
            _23 = yz - wx;
            _24 = 0;

            _31 = xz - wy;
            _32 = yz + wx;
            _33 = 1 - (xx + yy);
            _34 = 0;

            _41 = 0;
            _42 = 0;
            _43 = 0;
            _44 = 1;
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

            d = 1.0f / d;

            m._11 = d * (_23 *_34 * _42 - _24 * _33 * _42 + _24 * _32 * _43 - _22 * _34 * _43 - _23 * _32 * _44 + _22 * _33 * _44);
            m._12 = d * (_14 *_33 * _42 - _13 * _34 * _42 - _14 * _32 * _43 + _12 * _34 * _43 + _13 * _32 * _44 - _12 * _33 * _44);
            m._13 = d * (_13 *_24 * _42 - _14 * _23 * _42 + _14 * _22 * _43 - _12 * _24 * _43 - _13 * _22 * _44 + _12 * _23 * _44);
            m._14 = d * (_14 *_23 * _32 - _13 * _24 * _32 - _14 * _22 * _33 + _12 * _24 * _33 + _13 * _22 * _34 - _12 * _23 * _34);
            m._21 = d * (_24 *_33 * _41 - _23 * _34 * _41 - _24 * _31 * _43 + _21 * _34 * _43 + _23 * _31 * _44 - _21 * _33 * _44);
            m._22 = d * (_13 *_34 * _41 - _14 * _33 * _41 + _14 * _31 * _43 - _11 * _34 * _43 - _13 * _31 * _44 + _11 * _33 * _44);
            m._23 = d * (_14 *_23 * _41 - _13 * _24 * _41 - _14 * _21 * _43 + _11 * _24 * _43 + _13 * _21 * _44 - _11 * _23 * _44);
            m._24 = d * (_13 *_24 * _31 - _14 * _23 * _31 + _14 * _21 * _33 - _11 * _24 * _33 - _13 * _21 * _34 + _11 * _23 * _34);
            m._31 = d * (_22 *_34 * _41 - _24 * _32 * _41 + _24 * _31 * _42 - _21 * _34 * _42 - _22 * _31 * _44 + _21 * _32 * _44);
            m._32 = d * (_14 *_32 * _41 - _12 * _34 * _41 - _14 * _31 * _42 + _11 * _34 * _42 + _12 * _31 * _44 - _11 * _32 * _44);
            m._33 = d * (_12 *_24 * _41 - _14 * _22 * _41 + _14 * _21 * _42 - _11 * _24 * _42 - _12 * _21 * _44 + _11 * _22 * _44);
            m._34 = d * (_14 *_22 * _31 - _12 * _24 * _31 - _14 * _21 * _32 + _11 * _24 * _32 + _12 * _21 * _34 - _11 * _22 * _34);
            m._41 = d * (_23 *_32 * _41 - _22 * _33 * _41 - _23 * _31 * _42 + _21 * _33 * _42 + _22 * _31 * _43 - _21 * _32 * _43);
            m._42 = d * (_12 *_33 * _41 - _13 * _32 * _41 + _13 * _31 * _42 - _11 * _33 * _42 - _12 * _31 * _43 + _11 * _32 * _43);
            m._43 = d * (_13 *_22 * _41 - _12 * _23 * _41 - _13 * _21 * _42 + _11 * _23 * _42 + _12 * _21 * _43 - _11 * _22 * _43);
            m._44 = d * (_12 *_23 * _31 - _13 * _22 * _31 + _13 * _21 * _32 - _11 * _23 * _32 - _12 * _21 * _33 + _11 * _22 * _33);


            return m;
		}

		T determinant() const
		{
			return
                _14 * _23 * _32 * _41 - _13 * _24 * _32 * _41 - _14 * _22 * _33 * _41 + _12 * _24 * _33 * _41 +
                _13 * _22 * _34 * _41 - _12 * _24 * _34 * _41 - _14 * _23 * _31 * _42 + _13 * _24 * _31 * _42 +
                _14 * _21 * _33 * _42 - _11 * _24 * _33 * _42 - _13 * _21 * _34 * _42 + _11 * _23 * _34 * _42 +
                _14 * _22 * _31 * _43 - _12 * _24 * _31 * _43 - _14 * _21 * _32 * _43 + _11 * _24 * _32 * _43 +
                _12 * _21 * _34 * _43 - _11 * _22 * _34 * _43 - _13 * _22 * _31 * _44 + _12 * _23 * _31 * _44 +
                _13 * _21 * _32 * _44 - _11 * _23 * _32 * _44 - _12 * _21 * _33 * _44 + _11 * _22 * _33 * _44;
		}

		void identify()
		{
			_11 = 1, _12 = 0, _13 = 0, _14 = 0;
			_21 = 0, _22 = 1, _23 = 0, _24 = 0;
			_31 = 0, _32 = 0, _33 = 1, _34 = 0;
			_41 = 0, _42 = 0, _43 = 0, _44 = 1;
		}

        void translate(T x, T y, T z)
        {
            *this *= transformMatrix(x, y, z);
        }

        void scale(T x, T y, T z)
        {
            *this *= scaleMatrix(x, y, z);
        }

	public:
		static Matrix4x4<T> scaleMatrix(T x, T y, T z)
		{
			Matrix4x4<T> m;

			m._11 = x;
			m._22 = y;
			m._33 = z;

			return m;
		}

		static Matrix4x4<T> transformMatrix(T x, T y, T z)
		{
			Matrix4x4<T> m;

			m._41 = x;
			m._42 = y;
			m._43 = z;

			return m;
		}

		static Matrix4x4<T> rotationMatrixXAxis(T angle)
		{
			Matrix4x4<T> m;
            m._22 = MathUtilities::cos<T>(angle);
            m._23 = -MathUtilities::sin<T>(angle);
            m._32 = MathUtilities::sin<T>(angle);
            m._33 = MathUtilities::cos<T>(angle);
			return m;
		}

        static Matrix4x4<T> rotationMatrixYAxis(T angle)
        {
            Matrix4x4<T> m;

            m._11 = MathUtilities::cos<T>(angle);
            m._13 = MathUtilities::sin<T>(angle);
            m._31 = -MathUtilities::sin<T>(angle);
            m._33 = MathUtilities::cos<T>(angle);

            return m;
        }

        static Matrix4x4<T> rotationMatrixZAxis(T angle)
        {
            Matrix4x4<T> m;

            m._11 = MathUtilities::cos<T>(angle);
            m._12 = -MathUtilities::sin<T>(angle);
            m._21 = MathUtilities::sin<T>(angle);
            m._22 = MathUtilities::cos<T>(angle);

            return m;
        }

        static Matrix4x4<T> rotationMatrix(Vector3<T> axis, float angle)
        {
            axis = axis * MathUtilities::sin<T>(angle * 0.5f);
            return Matrix4x4<T>(Quaternion<T>(axis._x, axis._y, axis._z, MathUtilities::cos<float>(angle * 0.5)));
        }

        static Matrix4x4<T> perspectiveMatrix(float fov, float aspect, float n, float f)
        {
            Matrix4x4<T> m;

            float yScale = MathUtilities::cot(fov / 2.0);
            float xScale = yScale / aspect;

            m._11 = xScale;
            m._22 = yScale;
            m._33 = f / (f - n);
            m._43 = -n * f / (f - n);
            m._34 = 1.0;

            return m;
        }

        static Matrix4x4<T> orthoMatrix(float w, float h, float n, float f)
        {
            Matrix4x4<T> m;

            m._11 = 2.0 / w;
            m._22 = 2.0 / h;
            m._33 = 1.0 / (f - n);
            m._43 = -n / (f - n);
            m._44 = 1.0;

            return m;
        }

        static Matrix4x4<T> viewMatrix(Vector3<T> pos, Vector3<T> lookAt, Vector3<T> up)
        {
            Matrix4x4<T> m;

            //TODO

            return m;
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
