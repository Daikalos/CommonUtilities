#pragma once

#include <array>

#include <cassert>
#include <cstddef>

#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
    template <IsEnum Enum, typename T, std::size_t Count>
    struct EnumArray : public std::array<T, Count>
    {
        NODISC CONSTEXPR const T& operator[](Enum aKey) const;
        NODISC CONSTEXPR T& operator[](Enum aKey);
    };

    template<IsEnum Enum, typename T, std::size_t Count>
    CONSTEXPR T& EnumArray<Enum, T, Count>::operator[](Enum aKey)
    {
        const auto index = static_cast<std::size_t>(aKey);
        assert(index < Count && "Index is out of bounds");
        return std::array<T, Count>::operator[](index);
    }

    template<IsEnum Enum, typename T, std::size_t Count>
    CONSTEXPR const T& EnumArray<Enum, T, Count>::operator[](Enum aKey) const
    {
        const auto index = static_cast<std::size_t>(aKey);
        assert(index < Count && "Index is out of bounds");
        return std::array<T, Count>::operator[](index);
    }
}