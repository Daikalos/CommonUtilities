#pragma once

#include <vector>

#include <CommonUtilities/Math/Vector.hpp>
#include <CommonUtilities/Structures/StaticVector.hpp>
#include <CommonUtilities/Utility/JsonUtils.hpp>

// THIS CLASS IS A TRANSLATION TO C++11 FROM THE REFERENCE
// JAVA IMPLEMENTATION OF THE IMPROVED PERLIN FUNCTION (see http://mrl.nyu.edu/~perlin/noise/)
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN

// I ADDED AN EXTRA METHOD THAT GENERATES A NEW PERMUTATION VECTOR (THIS IS NOT PRESENT IN THE ORIGINAL IMPLEMENTATION)

namespace CommonUtilities
{
	enum class NoiseQuality
	{
		Low,
		Medium,
		High,
		Count
	};

	template<typename T>
    struct NoiseSample
    {
        float value = 0.0f;
        T derivative;

		constexpr operator float() const
		{
			return value;
		}
    };

	using NoiseSample3D = NoiseSample<Vector3f>;
	using NoiseSample2D = NoiseSample<Vector2f>;
	using NoiseSample1D = NoiseSample<float>;

    class PerlinNoise
    {
    public:
        // Initialize with the reference values for the permutation vector
        PerlinNoise();

        // Generate a new permutation vector based on the value of seed
        PerlinNoise(unsigned int seed);

		unsigned GetSeed() const;

		void SetSeed(unsigned aSeed);

		NoiseSample1D Noise1D(float aPoint, float aFrequency = 1.0f) const;

		NoiseSample1D Noise1D(float aPoint, float aFrequency, int aOctaves, float aOctaveMultiplier = 1.0f, float aOctaveScale = 1.0f) const;

		NoiseSample2D Noise2D(const cu::Vector2f& aPoint, float aFrequency = 1.0f) const;

		NoiseSample2D Noise2D(const cu::Vector2f& aPoint, float aFrequency, int aOctaves, float aOctaveMultiplier = 1.0f, float aOctaveScale = 1.0f) const;

        /// Returns value in the range [-1.0f, 1.0f]
        /// 
		NoiseSample3D Noise3D(const cu::Vector3f& aPoint, float aFrequency = 1.0f) const;

		NoiseSample3D Noise3D(const cu::Vector3f& aPoint, float aFrequency, int aOctaves, float aOctaveMultiplier = 1.0f, float aOctaveScale = 1.0f) const;

		Vector3f Curl(const cu::Vector3f& aPoint, float aFrequency = 1.0f, const Vector3f& aStrength = Vector3f{ 1.0f }, NoiseQuality aQuality = NoiseQuality::High) const;

		Vector3f Curl(const cu::Vector3f& aPoint, float aFrequency, const Vector3f& aStrength, int aOctaves, float aOctaveMultiplier = 1.0f, float aOctaveScale = 1.0f, NoiseQuality aQuality = NoiseQuality::High) const;

    private:
		static constexpr std::size_t ourTableSize = 256;

        float Fade(float t) const;
        float FadeDerivate(float t) const;

        // The permutation vector
        cu::StaticVector<int, ourTableSize * 2>	myTable;
		std::array<cu::Vector3f, ourTableSize>	myGradients;
		std::array<cu::Vector2f, ourTableSize>	myGradients2D;
		std::array<float, ourTableSize>			myGradients1D;
		unsigned								mySeed = 0;

		friend void to_json(nlohmann::json& nlohmann_json_j, const PerlinNoise& nlohmann_json_t)
		{
			nlohmann_json_j["mySeed"] = nlohmann_json_t.mySeed;
		} 
		friend void from_json(const nlohmann::json& nlohmann_json_j, PerlinNoise& nlohmann_json_t)
		{
			unsigned seed = 0;
			nlohmann_json_j.at("mySeed").get_to(seed);

			nlohmann_json_t.mySeed = 0;
			nlohmann_json_t.SetSeed(seed);
		}
    };

	template<typename T>
	constexpr NoiseSample<T>& operator+=(NoiseSample<T>& aLeft, const NoiseSample<T>& aRight)
	{
		aLeft.value += aRight.value;
		aLeft.derivative += aRight.derivative;

		return aLeft;
	}
	template<typename T>
	constexpr NoiseSample<T>& operator-=(NoiseSample<T>& aLeft, const NoiseSample<T>& aRight)
	{
		aLeft.value -= aRight.value;
		aLeft.derivative -= aRight.derivative;

		return aLeft;
	}

	template<typename T>
	constexpr NoiseSample<T> operator+(NoiseSample<T> aLeft, float aRight)
	{
		aLeft.value += aRight;
		return aLeft;
	}
	template<typename T>
	constexpr NoiseSample<T> operator+(float aLeft, NoiseSample<T> aRight)
	{
		aRight.value += aLeft;
		return aRight;
	}
	template<typename T>
	constexpr NoiseSample<T> operator+(NoiseSample<T> aLeft, NoiseSample<T> aRight) 
	{
		aLeft.value += aRight.value;
		aLeft.derivative += aRight.derivative;
		return aLeft;
	}

	template<typename T>
	constexpr NoiseSample<T> operator-(NoiseSample<T> aLeft, float aRight)
	{
		aLeft.value -= aRight;
		return aLeft;
	}
	template<typename T>
	constexpr NoiseSample<T> operator-(float aLeft, NoiseSample<T> aRight)
	{
		aRight.value = aLeft - aRight.value;
		aRight.derivative = -aRight.derivative;
		return aRight;
	}
	template<typename T>
	constexpr NoiseSample<T> operator-(NoiseSample<T> aLeft, NoiseSample<T> aRight)
	{
		aLeft.value -= aRight.value;
		aLeft.derivative -= aRight.derivative;
		return aLeft;
	}

	template<typename T>
	constexpr NoiseSample<T> operator*(NoiseSample<T> aLeft, float aRight)
	{
		aLeft.value *= aRight;
		aLeft.derivative *= aRight;
		return aLeft;
	}
	template<typename T>
	constexpr NoiseSample<T> operator*(float aLeft, NoiseSample<T> aRight)
	{
		return aRight * aLeft;
	}
	template<typename T>
	constexpr NoiseSample<T> operator*(NoiseSample<T> aLeft, NoiseSample<T> aRight)
	{
		aLeft.derivative = aLeft.derivative * aRight.value + aRight.derivative * aLeft.value;
		aLeft.value *= aRight.value;
		return aLeft;
	}
}