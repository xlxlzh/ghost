#ifndef _MATH_UTILITIES_H_
#define _MATH_UTILITIES_H_

#include <cmath>

namespace ghost
{
	class MathUtilities
	{
	public:
		template<typename T>
		static bool equal(const T& lhs, const T& rhs)
		{
			return abs(lhs - rhs) < PRECISION_FLOAT;
		}

		template<typename T>
		static T sqrt(const T x)
		{
			return ::sqrtf(static_cast<float>(x));
		}

		template<>
		static float sqrt(const float x)
		{
			return ::sqrtf(x);
		}

		template<>
		static double sqrt(const double x)
		{
			return ::sqrt(x);
		}

        template<typename T>
        static T sin(T angle)
        {
            float radian = static_cast<float>(angle * PI / 180.0f);
            return ::sinf(radian);
        }

        template<>
        static double sin(double angle)
        {
            double radian = static_cast<double>(angle * PI / 180.0f);
            return ::sin(radian);
        }

        template<typename T>
        static T cos(T angle)
        {
            float radian = static_cast<float>(angle * PI / 180.0f);
            return ::cosf(radian);
        }

        template<>
        static double cos(double angle)
        {
            double radian = static_cast<double>(angle * PI / 180.0f);
            return ::cos(radian);
        }

        template<typename T>
        static T tan(T angle)
        {
            float radian = static_cast<float>(angle * PI / 180.0f);
            return ::tanf(static_cast<float>(radian));
        }

        template<>
        static double tan(double angle)
        {
            double radian = static_cast<double>(angle * PI / 180.0f);
            return ::tan(radian);
        }

        template<typename T>
        static T cot(T angle)
        {
            float radian = static_cast<float>(angle * PI / 180.0f);
            float ret =  ::tanf(static_cast<float>(radian));
            return 1.0 / ret;
        }

        template<>
        static double cot(double angle)
        {
            double radian = static_cast<double>(angle * PI / 180.0f);
            double ret = ::tan(radian);
            return 1.0 / ret;
        }

	public:
		static float PRECISION_FLOAT;
        static float PI;
	};
}

#endif