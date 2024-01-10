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
		CONSTEXPR PlaneVolume();
		CONSTEXPR ~PlaneVolume();

		CONSTEXPR PlaneVolume(const std::vector<Plane<T>>& somePlanes);
		CONSTEXPR PlaneVolume(std::vector<Plane<T>>&& somePlanes);

		CONSTEXPR const Plane<T>& operator[](std::size_t aIndex) const;
		CONSTEXPR Plane<T>& operator[](std::size_t aIndex);

		CONSTEXPR const Plane<T>& Get(std::size_t aIndex) const;
		CONSTEXPR Plane<T>& Get(std::size_t aIndex);

		CONSTEXPR std::size_t Count() const noexcept;
		CONSTEXPR bool IsEmpty() const noexcept;

		template<typename... Args>
		CONSTEXPR void Emplace(Args&&... someArgs);

		CONSTEXPR void Add(const Plane<T>& aPlane);
		CONSTEXPR void Remove(std::size_t aIndex);

		CONSTEXPR bool IsInside(const Vector3<T>& aPosition) const;

		CONSTEXPR void Clear();

		auto GetType() const noexcept -> Type override;

	private:
		std::vector<Plane<T>> myPlanes;
	};

	template<typename T>
	CONSTEXPR PlaneVolume<T>::PlaneVolume() = default;

	template<typename T>
	CONSTEXPR PlaneVolume<T>::~PlaneVolume() = default;

	template<typename T>
	CONSTEXPR PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>>& somePlanes) 
		: myPlanes(somePlanes)
	{

	}

	template<typename T>
	CONSTEXPR PlaneVolume<T>::PlaneVolume(std::vector<Plane<T>>&& somePlanes)
		: myPlanes(std::move(somePlanes))
	{

	}

	template<typename T>
	CONSTEXPR const Plane<T>& PlaneVolume<T>::operator[](std::size_t aIndex) const
	{
		return GetPlane(aIndex);
	}
	template<typename T>
	CONSTEXPR Plane<T>& PlaneVolume<T>::operator[](std::size_t aIndex)
	{
		return GetPlane(aIndex);
	}

	template<typename T>
	CONSTEXPR const Plane<T>& PlaneVolume<T>::Get(std::size_t aIndex) const
	{
		return myPlanes[aIndex];
	}
	template<typename T>
	CONSTEXPR Plane<T>& PlaneVolume<T>::Get(std::size_t aIndex)
	{
		return myPlanes[aIndex];
	}

	template<typename T>
	CONSTEXPR std::size_t PlaneVolume<T>::Count() const noexcept
	{
		return myPlanes.size();
	}
	template<typename T>
	CONSTEXPR bool PlaneVolume<T>::IsEmpty() const noexcept
	{
		return myPlanes.empty();
	}

	template<typename T>
	template<typename... Args>
	CONSTEXPR void PlaneVolume<T>::Emplace(Args&&... someArgs)
	{
		myPlanes.emplace_back(std::forward<Args>(someArgs)...);
	}

	template<typename T>
	CONSTEXPR void PlaneVolume<T>::Add(const Plane<T>& aPlane)
	{
		EmplacePlane(aPlane);
	}
	template<typename T>
	CONSTEXPR void PlaneVolume<T>::Remove(std::size_t aIndex)
	{
		myPlanes.erase(myPlanes.begin() + aIndex);
	}

	template<typename T>
	CONSTEXPR bool PlaneVolume<T>::IsInside(const Vector3<T>& aPosition) const
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
	CONSTEXPR void PlaneVolume<T>::Clear()
	{
		myPlanes.clear();
	}

	template<typename T>
	inline auto PlaneVolume<T>::GetType() const noexcept -> Type
	{
		return Type::PlaneVolume;
	}
}