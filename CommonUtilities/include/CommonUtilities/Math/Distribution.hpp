#pragma once

#include <functional>

#include <CommonUtilities/Math/Vector2.hpp>
#include <CommonUtilities/Math/Vector3.hpp>
#include <CommonUtilities/Utility/Traits.h>

namespace CommonUtilities
{
	template<typename T>
	class Distribution
	{
	public:
		Distribution(const Distribution&) = default;
		Distribution(Distribution&&) = default;

		Distribution& operator=(const Distribution&) = default;
		Distribution& operator=(Distribution&&) = default;

		template<std::convertible_to<T> U>
		Distribution(U&& aConstant);

		template<std::convertible_to<T> U>
		Distribution(const Distribution<U>& aDistribution);

		template<typename Func>
		Distribution(Func&& aFunction) requires (!std::convertible_to<Func, T> && !std::same_as<std::decay_t<Func>, Distribution>);

		T operator()() const;

	private:
		std::function<T()> myFactory;
	};

	template<typename T>
	template<std::convertible_to<T> U>
	inline Distribution<T>::Distribution(U&& aConstant)
		: myFactory(
			[constant = std::forward<U>(aConstant)]() -> T
			{ 
				return constant; 
			})
	{

	}

	template<typename T>
	template<std::convertible_to<T> U>
	inline Distribution<T>::Distribution(const Distribution<U>& aDistribution)
		: myFactory(aDistribution)
	{

	}

	template<typename T>
	template<typename Func>
	inline Distribution<T>::Distribution(Func&& aFunction) requires (!std::convertible_to<Func, T> && !std::same_as<std::decay_t<Func>, Distribution>)
		: myFactory(std::forward<Func>(aFunction))
	{

	}

	template<typename T>
	inline T Distribution<T>::operator()() const
	{
		return myFactory();
	}

	COMMON_UTILITIES_API NODISC Distribution<int> CreateDistributionUniform(int aMin, int aMax);

	COMMON_UTILITIES_API NODISC Distribution<unsigned int> CreateDistributionUniform(unsigned int aMin, unsigned int aMax);

	COMMON_UTILITIES_API NODISC Distribution<float> CreateDistributionUniform(float aMin, float aMax);

	COMMON_UTILITIES_API NODISC Distribution<cu::Vector2f> CreateDistributionRect(const cu::Vector2f& aCenter, const cu::Vector2f& aHalfSize);

	COMMON_UTILITIES_API NODISC Distribution<cu::Vector2f> CreateDistributionCircle(const cu::Vector2f& aCenter, float aRadius);

	COMMON_UTILITIES_API NODISC Distribution<cu::Vector2f> CreateDistributionDeflect(const cu::Vector2f& aDirection, float aMaxRotation);

	COMMON_UTILITIES_API NODISC Distribution<cu::Vector3f> CreateDistributionRect(const cu::Vector3f& aCenter, const cu::Vector3f& aHalfSize);

	COMMON_UTILITIES_API NODISC Distribution<cu::Vector3f> CreateDistributionSphere(const cu::Vector3f& aCenter, float aRadius);
}