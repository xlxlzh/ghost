#ifndef _MATH_UTILITIES_H_
#define _MATH_UTILITIES_H_

#include <cmath>
#include "Ghost.h"

namespace ghost
{
	class GHOST_API MathUtilities
	{
	public:
		template<typename T>
		static bool Equal(const T& lhs, const T& rhs)
		{
			return abs(lhs - rhs) < PRECISION_FLOAT;
		}

		template<typename T>
		static T Sqrt(const T x)
		{
			return ::sqrtf(static_cast<float>(x));
		}

		template<>
		static float Sqrt(const float x)
		{
			return ::sqrtf(x);
		}

		template<>
		static double Sqrt(const double x)
		{
			return ::sqrt(x);
		}

        template<typename T>
        static T Sin(T angle)
        {
            float radian = static_cast<float>(angle * PI / 180.0f);
            return ::sinf(radian);
        }

        template<>
        static double Sin(double angle)
        {
            double radian = static_cast<double>(angle * PI / 180.0f);
            return ::sin(radian);
        }

        template<typename T>
        static T Cos(T angle)
        {
            float radian = static_cast<float>(angle * PI / 180.0f);
            return ::cosf(radian);
        }

        template<>
        static double Cos(double angle)
        {
            double radian = static_cast<double>(angle * PI / 180.0f);
            return ::cos(radian);
        }

        template<typename T>
        static T Tan(T angle)
        {
            float radian = static_cast<float>(angle * PI / 180.0f);
            return ::tanf(static_cast<float>(radian));
        }

        template<>
        static double Tan(double angle)
        {
            double radian = static_cast<double>(angle * PI / 180.0f);
            return ::tan(radian);
        }

        template<typename T>
        static T Cot(T angle)
        {
            float radian = static_cast<float>(angle * PI / 180.0f);
            float ret =  ::tanf(static_cast<float>(radian));
            return 1.0f / ret;
        }

        template<>
        static double Cot(double angle)
        {
            double radian = static_cast<double>(angle * PI / 180.0f);
            double ret = ::tan(radian);
            return 1.0f / ret;
        }

	public:
		static float PRECISION_FLOAT;
        static float PI;
	};
}

#endif