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

	public:
		static float PRECISION_FLOAT;
	};

	float MathUtilities::PRECISION_FLOAT = 1.0e-10f;
}