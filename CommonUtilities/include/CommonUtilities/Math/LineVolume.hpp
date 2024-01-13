#pragma once

#include <vector>

#include "Line.hpp"
#include "Shape.h"

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<typename T>
	class LineVolume final : public Shape
	{
	public:
		CONSTEXPR LineVolume() = default;
		CONSTEXPR ~LineVolume() = default;

		CONSTEXPR LineVolume(const std::vector<Line<T>>& someLines);
		CONSTEXPR LineVolume(std::vector<Line<T>>&& someLines);

		NODISC CONSTEXPR const Line<T>& operator[](std::size_t aIndex) const;
		NODISC CONSTEXPR Line<T>& operator[](std::size_t aIndex);

		NODISC CONSTEXPR const Line<T>& Get(std::size_t aIndex) const;
		NODISC CONSTEXPR Line<T>& Get(std::size_t aIndex);

		NODISC CONSTEXPR std::size_t Count() const noexcept;
		NODISC CONSTEXPR bool IsEmpty() const noexcept;

		template<typename... Args>
		CONSTEXPR void Emplace(Args&&... someArgs);

		CONSTEXPR void Add(const Line<T>& aLine);
		CONSTEXPR void Remove(std::size_t aIndex);

		NODISC CONSTEXPR bool IsInside(const Vector2<T>& aPosition) const;

		CONSTEXPR void Clear();

		NODISC CONSTEXPR auto GetType() const noexcept -> Type override;

	private:
		std::vector<Line<T>> myLines;
	};

	template<typename T>
	CONSTEXPR LineVolume<T>::LineVolume(const std::vector<Line<T>>& someLines) 
		: myLines(someLines)
	{

	}

	template<typename T>
	CONSTEXPR LineVolume<T>::LineVolume(std::vector<Line<T>>&& someLines) 
		: myLines(std::move(someLines))
	{

	}

	template<typename T>
	CONSTEXPR const Line<T>& LineVolume<T>::operator[](std::size_t aIndex) const
	{
		return GetLine(aIndex);
	}
	template<typename T>
	CONSTEXPR Line<T>& LineVolume<T>::operator[](std::size_t aIndex)
	{
		return GetLine(aIndex);
	}

	template<typename T>
	CONSTEXPR const Line<T>& LineVolume<T>::Get(std::size_t aIndex) const
	{
		return myLines[aIndex];
	}
	template<typename T>
	CONSTEXPR Line<T>& LineVolume<T>::Get(std::size_t aIndex)
	{
		return myLines[aIndex];
	}

	template<typename T>
	CONSTEXPR std::size_t LineVolume<T>::Count() const noexcept
	{
		return myLines.size();
	}
	template<typename T>
	CONSTEXPR bool LineVolume<T>::IsEmpty() const noexcept
	{
		return myLines.empty();
	}

	template<typename T>
	template<typename...Args>
	CONSTEXPR void LineVolume<T>::Emplace(Args&&... someArgs)
	{
		myLines.emplace_back(std::forward<Args>(someArgs)...);
	}

	template<typename T>
	CONSTEXPR void LineVolume<T>::Add(const Line<T>& aLine)
	{
		EmplaceLine(aLine);
	}
	template<typename T>
	CONSTEXPR void LineVolume<T>::Remove(std::size_t aIndex)
	{
		myLines.erase(myLines.begin() + aIndex);
	}

	template<typename T>
	CONSTEXPR bool LineVolume<T>::IsInside(const Vector2<T>& aPosition) const
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
	CONSTEXPR void LineVolume<T>::Clear()
	{
		myLines.clear();
	}

	template<typename T>
	CONSTEXPR auto LineVolume<T>::GetType() const noexcept -> Type
	{
		return Type::LineVolume;
	}
}