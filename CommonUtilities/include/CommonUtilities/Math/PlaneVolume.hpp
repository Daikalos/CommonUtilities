#pragma once

#include <vector>

#include "Plane.hpp"
#include "Shape.h"

namespace CommonUtilities
{
	template<typename T>
	class PlaneVolume : public Shape
	{
	public:
		PlaneVolume();
		~PlaneVolume();

		PlaneVolume(const std::vector<Plane<T>>& somePlanes);

		const Plane<T>& operator[](std::size_t aIndex) const;
		Plane<T>& operator[](std::size_t aIndex);

		const Plane<T>& GetPlane(std::size_t aIndex) const;
		Plane<T>& GetPlane(std::size_t aIndex);

		auto GetType() const noexcept -> Type override;

		template<typename... Args>
		void EmplacePlane(Args&&... someArgs);

		void AddPlane(const Plane<T>& aPlane);
		void RemovePlane(std::size_t aIndex);

		bool IsInside(const Vector3<T>& aPosition) const;

		void Clear();

	private:
		std::vector<Plane<T>> myPlanes;
	};

	template<typename T>
	inline PlaneVolume<T>::PlaneVolume() = default;

	template<typename T>
	inline PlaneVolume<T>::~PlaneVolume() = default;

	template<typename T>
	inline PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>>& somePlanes) : myPlanes(somePlanes)
	{

	}

	template<typename T>
	inline const Plane<T>& PlaneVolume<T>::operator[](std::size_t aIndex) const
	{
		return GetPlane(aIndex);
	}
	template<typename T>
	inline Plane<T>& PlaneVolume<T>::operator[](std::size_t aIndex)
	{
		return GetPlane(aIndex);
	}

	template<typename T>
	inline const Plane<T>& PlaneVolume<T>::GetPlane(std::size_t aIndex) const
	{
		return myPlanes[aIndex];
	}
	template<typename T>
	inline Plane<T>& PlaneVolume<T>::GetPlane(std::size_t aIndex)
	{
		return myPlanes[aIndex];
	}

	template<typename T>
	inline auto PlaneVolume<T>::GetType() const noexcept -> Type
	{
		return Type::PlaneVolume;
	}

	template<typename T>
	template<typename... Args>
	inline void PlaneVolume<T>::EmplacePlane(Args&&... someArgs)
	{
		myPlanes.emplace_back(std::forward<Args>(someArgs)...);
	}

	template<typename T>
	inline void PlaneVolume<T>::AddPlane(const Plane<T>& aPlane)
	{
		EmplacePlane(aPlane);
	}
	template<typename T>
	inline void PlaneVolume<T>::RemovePlane(std::size_t aIndex)
	{
		myPlanes.erase(myPlanes.begin() + aIndex);
	}

	template<typename T>
	inline bool PlaneVolume<T>::IsInside(const Vector3<T>& aPosition) const
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
	inline void PlaneVolume<T>::Clear()
	{
		myPlanes.clear();
	}
}