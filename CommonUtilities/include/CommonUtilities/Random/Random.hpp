#pragma once

#include <random>
#include <type_traits>
#include <numeric>

#include <CommonUtilities/Utility/ArithmeticUtils.hpp>
#include <CommonUtilities/Structures/EnumArray.hpp>
#include <CommonUtilities/Math/Vector2.hpp>
#include <CommonUtilities/Math/Vector3.hpp>
#include <CommonUtilities/Math/Quaternion.hpp>

#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	namespace priv
	{
		inline thread_local std::uint64_t Seed = std::random_device{}();
	}

	inline thread_local std::mt19937_64 dre(priv::Seed);

	NODISC inline float Random()
	{
		std::uniform_real_distribution<float> uid(0.0f, 1.0f);
		return uid(dre);
	}

	template<IsFloatingPointType T>
	NODISC inline T Random(T aMin, T aMax)
	{
		std::uniform_real_distribution<T> uid(aMin, aMax);
		return uid(dre);
	}
	template<IsIntegralType T>
	NODISC inline T Random(T aMin, T aMax)
	{
		std::uniform_int_distribution<T> uid(aMin, aMax);
		return uid(dre);
	}

	template<typename T>
	NODISC inline T RandomDev(T aMiddle, T aDeviation)
	{
		return Random<T>(aMiddle - aDeviation, aMiddle + aDeviation);
	}

	template<typename T, typename... Args>
	NODISC inline T RandomArg(T&& aFirstArg, Args&&... someArgs)
	{
		constexpr std::size_t size = sizeof...(someArgs) + 1;

		const T arr[size] = { std::forward<T>(aFirstArg), std::forward<Args>(someArgs)... };
		return arr[Random<std::size_t>(0LLU, size - 1)];
	}

	///	Creates a numerically increasing list of values up to size that are then shuffled and returned.
	/// 
	template<typename T>
	NODISC inline auto RandomVector(T size)
	{
		std::vector<T> result;
		result.resize(size);

		std::iota(result.begin(), result.end(), 0);
		std::shuffle(result.begin(), result.end(), dre);

		return result;
	}

	///	Creates a numerically increasing list of values up to size that are then shuffled and returned.
	/// 
	template<typename T, std::size_t N>
	NODISC inline auto RandomArray()
	{
		std::array<T, N> result;

		std::iota(result.begin(), result.end(), 0);
		std::shuffle(result.begin(), result.end(), dre);

		return result;
	}

	template<typename T>
	NODISC inline Vector2<T> RandomPointInRect(const Vector2<T>& aCenter, const Vector2<T>& aHalfSize)
	{
		return Vector2<T>(
			RandomDev<T>(aCenter.x, aHalfSize.x), 
			RandomDev<T>(aCenter.y, aHalfSize.y));
	}

	template<typename T>
	NODISC inline Vector2<T> RandomPointInCircle(const Vector2<T>& aCenter, T aRadius, T aInnerRadius = T())
	{
		const T a		= Random<T>(T(0), T(1)) * (aRadius * aRadius - aInnerRadius * aInnerRadius) + aInnerRadius * aInnerRadius;
		const T r		= (a ? std::sqrt(a) : 0.0f);
		const T theta	= Random<T>(T(0), T(1)) * TAU_V<T>;

		return Vector2<T>(
			(T)(aCenter.x + r * std::cos(theta)),
			(T)(aCenter.y + r * std::sin(theta)));
	}

	template<typename T>
	NODISC inline Vector2<T> RandomDeflection(const Vector2<T>& aDirection, T aMaxRotation)
	{
		T angle = RandomDev<T>(0.0f, aMaxRotation);

		T cos = std::cos(angle);
		T sin = std::sin(angle);

		return Vector2<T>(
			(T)(cos * aDirection.x - sin * aDirection.y),
			(T)(sin * aDirection.x + cos * aDirection.y));
	}

	template<typename T>
	NODISC inline Vector3<T> RandomPointOnSegment(const Vector3<T>& aStart, const Vector3<T>& aEnd)
	{
		return Vector3<T>::Lerp(aStart, aEnd, cu::Random());
	}

	template<typename T>
	NODISC inline Vector3<T> RandomPointInBox(const Vector3<T>& aCenter, const Vector3<T>& aHalfSize)
	{
		return Vector3<T>(
			RandomDev<T>(aCenter.x, aHalfSize.x),
			RandomDev<T>(aCenter.y, aHalfSize.y),
			RandomDev<T>(aCenter.z, aHalfSize.z));
	}

	template<typename T>
	NODISC inline Vector3<T> RandomPointInBoxShell(const Vector3<T>& aCenter, const Vector3<T>& aHalfSize)
	{
		enum class BoxSide
		{
			Front,
			Back,
			Right,
			Left,
			Top,
			Bot
		};

		EnumArray<BoxSide, T, 6> sidesWeights;

		sidesWeights[BoxSide::Front]	= sidesWeights[BoxSide::Back]	= aHalfSize.y * aHalfSize.x * T(4);
		sidesWeights[BoxSide::Right]	= sidesWeights[BoxSide::Left]	= aHalfSize.y * aHalfSize.z * T(4);
		sidesWeights[BoxSide::Top]		= sidesWeights[BoxSide::Bot]	= aHalfSize.x * aHalfSize.z * T(4);

		T totalArea		= std::accumulate(sidesWeights.begin(), sidesWeights.end(), T(0));
		T randomWeight	= Random(T(0), totalArea);

		BoxSide chosenSide		= BoxSide::Front;
		T		currentWeight	= T(0);

		for (std::size_t i = 0; i < sidesWeights.size(); ++i)
		{
			T sideWeight = sidesWeights[(BoxSide)i];

			currentWeight += sideWeight;
			if (currentWeight >= randomWeight)
			{
				chosenSide = (BoxSide)i;
				break;
			}
		}
		
		Vector3<T> randomPoint = aCenter;

		switch (chosenSide)
		{
			case BoxSide::Front:
			{
				randomPoint = Vector3<T>
				{
					RandomDev<T>(aCenter.x, aHalfSize.x),
					RandomDev<T>(aCenter.y, aHalfSize.y), 
					aCenter.z + aHalfSize.z 
				};

				break;
			}
			case BoxSide::Back:
			{
				randomPoint = Vector3<T>
				{
					RandomDev<T>(aCenter.x, aHalfSize.x),
					RandomDev<T>(aCenter.y, aHalfSize.y), 
					aCenter.z - aHalfSize.z 
				};

				break;
			}
			case BoxSide::Right:
			{
				randomPoint = Vector3<T>
				{
					aCenter.x + aHalfSize.x,
					RandomDev<T>(aCenter.y, aHalfSize.y), 
					RandomDev<T>(aCenter.z, aHalfSize.z) 
				};

				break;
			}
			case BoxSide::Left:
			{
				randomPoint = Vector3<T>
				{
					aCenter.x - aHalfSize.x,
					RandomDev<T>(aCenter.y, aHalfSize.y),
					RandomDev<T>(aCenter.z, aHalfSize.z)
				};

				break;
			}
			case BoxSide::Top:
			{
				randomPoint = Vector3<T>
				{
					RandomDev<T>(aCenter.x, aHalfSize.x),
					aCenter.y + aHalfSize.y,
					RandomDev<T>(aCenter.z, aHalfSize.z)
				};

				break;
			}
			case BoxSide::Bot:
			{
				randomPoint = Vector3<T>
				{
					RandomDev<T>(aCenter.x, aHalfSize.x),
					aCenter.y - aHalfSize.y,
					RandomDev<T>(aCenter.z, aHalfSize.z) 
				};

				break;
			}
		}

		return randomPoint;
	}

	template<typename T>
	NODISC inline Vector3<T> RandomPointInSphere(const Vector3<T>& aCenter, T aRadius, T aInnerRadius = T())
	{
		if (aRadius == T(0))
			return Vector3<T>::Zero;

		const T p		= Random<T>(aInnerRadius, aRadius);
		const T r		= p * std::cbrt(p / aRadius);
		const T theta	= Random<T>(T(0), T(1)) * TAU_V<T>;
		const T phi		= std::acos(Random<T>(-T(1), T(1)));

		const T sinTheta = std::sin(theta);
		const T cosTheta = std::cos(theta);

		const T sinPhi = std::sin(phi);
		const T cosPhi = std::cos(phi);

		return Vector3<T>(
			aCenter.x + (T)(r * sinPhi * cosTheta),
			aCenter.y + (T)(r * sinPhi * sinTheta),
			aCenter.z + (T)(r * cosPhi));
	}

	template<typename T>
	NODISC inline Vector3<T> RandomPointInCone(const Vector3<T>& aCenter, const cu::Quatf& aRotation, T aAngle, T aLength, T aRadius, T aInnerRadius = T())
	{
		const T rndLen = Random<T>(T(0), aLength);
		const T c = std::tan(aAngle);
		const T a = rndLen * c;
		const T b = aLength * c;

		Vector3<T> randomPoint = RandomPointInCircle<T>(Vector2<T>::Zero, aRadius + a, std::max(aInnerRadius - b + a, T(0))).XZY(rndLen);

		return aRotation * randomPoint + aCenter;
	}

	template<typename T>
	NODISC inline Vector2<T> RandomDirection2D()
	{
		Vector2<T> randomPoint = RandomPointInCircle<T>(Vector2<T>(), T(1));
		return randomPoint.GetNormalizedSafe();
	}

	template<typename T>
	NODISC inline Vector3<T> RandomDirection3D()
	{
		Vector3<T> randomPoint = RandomPointInSphere<T>(Vector3<T>(), T(1));
		return randomPoint.GetNormalizedSafe();
	}

	COMMON_UTILITIES_API void PushSeed(std::uint64_t seed = std::mt19937_64::default_seed);

	COMMON_UTILITIES_API void PopSeed();
}