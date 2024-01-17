#pragma once

#include <vector>

#include "Plane.hpp"
#include "Shape.h"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class PlaneVolume : public Shape
	{
	public:
		constexpr PlaneVolume() = default;
		constexpr ~PlaneVolume() = default;

		constexpr PlaneVolume(const std::vector<Plane<T>>& somePlanes);
		constexpr PlaneVolume(std::vector<Plane<T>>&& somePlanes);

		NODISC constexpr const Plane<T>& operator[](std::size_t aIndex) const;
		NODISC constexpr Plane<T>& operator[](std::size_t aIndex);

		NODISC constexpr const Plane<T>& Get(std::size_t aIndex) const;
		NODISC constexpr Plane<T>& Get(std::size_t aIndex);

		NODISC constexpr std::size_t Count() const noexcept;
		NODISC constexpr bool IsEmpty() const noexcept;

		template<typename... Args>
		constexpr void Emplace(Args&&... someArgs);

		constexpr void Add(const Plane<T>& aPlane);
		constexpr void Remove(std::size_t aIndex);

		NODISC constexpr bool IsInside(const Vector3<T>& aPosition) const;

		constexpr void Clear();

		NODISC constexpr auto GetType() const noexcept -> Type override;

	private:
		std::vector<Plane<T>> myPlanes;
	};

	template<typename T>
	constexpr PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>>& somePlanes) 
		: myPlanes(somePlanes)
	{

	}

	template<typename T>
	constexpr PlaneVolume<T>::PlaneVolume(std::vector<Plane<T>>&& somePlanes)
		: myPlanes(std::move(somePlanes))
	{

	}

	template<typename T>
	constexpr const Plane<T>& PlaneVolume<T>::operator[](std::size_t aIndex) const
	{
		return GetPlane(aIndex);
	}
	template<typename T>
	constexpr Plane<T>& PlaneVolume<T>::operator[](std::size_t aIndex)
	{
		return GetPlane(aIndex);
	}

	template<typename T>
	constexpr const Plane<T>& PlaneVolume<T>::Get(std::size_t aIndex) const
	{
		return myPlanes[aIndex];
	}
	template<typename T>
	constexpr Plane<T>& PlaneVolume<T>::Get(std::size_t aIndex)
	{
		return myPlanes[aIndex];
	}

	template<typename T>
	constexpr std::size_t PlaneVolume<T>::Count() const noexcept
	{
		return myPlanes.size();
	}
	template<typename T>
	constexpr bool PlaneVolume<T>::IsEmpty() const noexcept
	{
		return myPlanes.empty();
	}

	template<typename T>
	template<typename... Args>
	constexpr void PlaneVolume<T>::Emplace(Args&&... someArgs)
	{
		myPlanes.emplace_back(std::forward<Args>(someArgs)...);
	}

	template<typename T>
	constexpr void PlaneVolume<T>::Add(const Plane<T>& aPlane)
	{
		EmplacePlane(aPlane);
	}
	template<typename T>
	constexpr void PlaneVolume<T>::Remove(std::size_t aIndex)
	{
		myPlanes.erase(myPlanes.begin() + aIndex);
	}

	template<typename T>
	constexpr bool PlaneVolume<T>::IsInside(const Vector3<T>& aPosition) const
	{
		for (const Plane<T>& plane : myPlanes)
		{
			if (!plane.IsInside(aPosition))
			{
				return false;
			}
		}

		return true;
	}

	template<typename T>
	constexpr void PlaneVolume<T>::Clear()
	{
		myPlanes.clear();
	}

	template<typename T>
	constexpr auto PlaneVolume<T>::GetType() const noexcept -> Type
	{
		return Type::PlaneVolume;
	}

	using PlaneVolumeFloat	= PlaneVolume<float>;
	using PlaneVolumeDouble	= PlaneVolume<double>;
	using PlaneVolumeInt	= PlaneVolume<int>;
}