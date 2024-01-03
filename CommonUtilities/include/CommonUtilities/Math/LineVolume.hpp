#pragma once

#include <vector>

#include "Line.hpp"
#include "Shape.h"

namespace CommonUtilities
{
	template<typename T>
	class LineVolume final : public Shape
	{
	public:
		LineVolume();
		~LineVolume();

		LineVolume(const std::vector<Line<T>>& someLines);

		const Line<T>& operator[](std::size_t aIndex) const;
		Line<T>& operator[](std::size_t aIndex);

		const Line<T>& GetLine(std::size_t aIndex) const;
		Line<T>& GetLine(std::size_t aIndex);

		auto GetType() const noexcept -> Type override;

		template<typename... Args>
		void EmplaceLine(Args&&... someArgs);

		void AddLine(const Line<T>& aLine);
		void RemoveLine(std::size_t aIndex);

		bool IsInside(const Vector2<T>& aPosition) const;

		void Clear();

	private:
		std::vector<Line<T>> myLines;
	};

	template<typename T>
	inline LineVolume<T>::LineVolume() = default;

	template<typename T>
	inline LineVolume<T>::~LineVolume() = default;

	template<typename T>
	inline LineVolume<T>::LineVolume(const std::vector<Line<T>>& someLines) : myLines(someLines)
	{

	}

	template<typename T>
	inline const Line<T>& LineVolume<T>::operator[](std::size_t aIndex) const
	{
		return GetLine(aIndex);
	}
	template<typename T>
	inline Line<T>& LineVolume<T>::operator[](std::size_t aIndex)
	{
		return GetLine(aIndex);
	}

	template<typename T>
	inline const Line<T>& LineVolume<T>::GetLine(std::size_t aIndex) const
	{
		return myLines[aIndex];
	}
	template<typename T>
	inline Line<T>& LineVolume<T>::GetLine(std::size_t aIndex)
	{
		return myLines[aIndex];
	}

	template<typename T>
	inline auto LineVolume<T>::GetType() const noexcept -> Type
	{
		return Type::LineVolume;
	}

	template<typename T>
	template<typename...Args>
	inline void LineVolume<T>::EmplaceLine(Args&&... someArgs)
	{
		myLines.emplace_back(std::forward<Args>(someArgs)...);
	}

	template<typename T>
	inline void LineVolume<T>::AddLine(const Line<T>& aLine)
	{
		EmplaceLine(aLine);
	}
	template<typename T>
	inline void LineVolume<T>::RemoveLine(std::size_t aIndex)
	{
		myLines.erase(myLines.begin() + aIndex);
	}

	template<typename T>
	inline bool LineVolume<T>::IsInside(const Vector2<T>& aPosition) const
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
	inline void LineVolume<T>::Clear()
	{
		myLines.clear();
	}
}