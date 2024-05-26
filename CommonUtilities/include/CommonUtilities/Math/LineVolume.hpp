#pragma once

#include <vector>

#include "Line.hpp"
#include "Shape.h"

#include <CommonUtilities/Utility/Clonable.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class LineVolume final : public Clonable<Shape, LineVolume<T>>
	{
	public:
		constexpr LineVolume() = default;
		constexpr ~LineVolume() = default;

		constexpr LineVolume(const std::vector<Line<T>>& someLines);
		constexpr LineVolume(std::vector<Line<T>>&& someLines);

		NODISC constexpr const Line<T>& operator[](std::size_t aIndex) const;
		NODISC constexpr Line<T>& operator[](std::size_t aIndex);

		NODISC constexpr const Line<T>& Get(std::size_t aIndex) const;
		NODISC constexpr Line<T>& Get(std::size_t aIndex);

		NODISC constexpr std::size_t Count() const noexcept;
		NODISC constexpr bool IsEmpty() const noexcept;

		template<typename... Args>
		constexpr void Emplace(Args&&... someArgs);

		constexpr void Add(const Line<T>& aLine);
		constexpr void Remove(std::size_t aIndex);

		NODISC constexpr bool IsInside(const Vector2<T>& aPosition) const;

		constexpr void Clear();

		NODISC constexpr Shape::Type GetType() const noexcept override;

	private:
		std::vector<Line<T>> myLines;
	};

	template<typename T>
	constexpr LineVolume<T>::LineVolume(const std::vector<Line<T>>& someLines) 
		: myLines(someLines)
	{

	}

	template<typename T>
	constexpr LineVolume<T>::LineVolume(std::vector<Line<T>>&& someLines) 
		: myLines(std::move(someLines))
	{

	}

	template<typename T>
	constexpr const Line<T>& LineVolume<T>::operator[](std::size_t aIndex) const
	{
		return GetLine(aIndex);
	}
	template<typename T>
	constexpr Line<T>& LineVolume<T>::operator[](std::size_t aIndex)
	{
		return GetLine(aIndex);
	}

	template<typename T>
	constexpr const Line<T>& LineVolume<T>::Get(std::size_t aIndex) const
	{
		return myLines[aIndex];
	}
	template<typename T>
	constexpr Line<T>& LineVolume<T>::Get(std::size_t aIndex)
	{
		return myLines[aIndex];
	}

	template<typename T>
	constexpr std::size_t LineVolume<T>::Count() const noexcept
	{
		return myLines.size();
	}
	template<typename T>
	constexpr bool LineVolume<T>::IsEmpty() const noexcept
	{
		return myLines.empty();
	}

	template<typename T>
	template<typename...Args>
	constexpr void LineVolume<T>::Emplace(Args&&... someArgs)
	{
		myLines.emplace_back(std::forward<Args>(someArgs)...);
	}

	template<typename T>
	constexpr void LineVolume<T>::Add(const Line<T>& aLine)
	{
		EmplaceLine(aLine);
	}
	template<typename T>
	constexpr void LineVolume<T>::Remove(std::size_t aIndex)
	{
		myLines.erase(myLines.begin() + aIndex);
	}

	template<typename T>
	constexpr bool LineVolume<T>::IsInside(const Vector2<T>& aPosition) const
	{
		for (const Line<T>& line : myLines)
		{
			if (!line.IsInside(aPosition))
			{
				return false;
			}
		}

		return true;
	}

	template<typename T>
	constexpr void LineVolume<T>::Clear()
	{
		myLines.clear();
	}

	template<typename T>
	constexpr Shape::Type LineVolume<T>::GetType() const noexcept
	{
		return Shape::Type::LineVolume;
	}

	using LineVolumeFloat	= LineVolume<float>;
	using LineVolumeDouble	= LineVolume<double>;
	using LineVolumeInt		= LineVolume<int>;
}