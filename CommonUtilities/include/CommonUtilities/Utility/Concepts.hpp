#pragma once

#include <concepts>
#include <functional>

#include <CommonUtilities/Utility/Traits.h>
#include <CommonUtilities/Config.h>

#if !C20_SUPPORT
#error Concepts are a C++20 feature; please change the language standard or don't include the header that caused this message.
#endif

namespace CommonUtilities
{
	template<typename T>
	concept IsArithmeticType = std::is_arithmetic_v<T>;

	template<typename T>
	concept IsIntegralType = std::is_integral_v<T>;

	template<typename T>
	concept IsFloatingPointType = std::is_floating_point_v<T>;

	template<typename T>
	concept IsEnumType = std::is_enum_v<T>;

	template<typename T>
	concept IsArithEnumType = IsArithmeticType<T> || IsEnumType<T>;

	template<typename T>
	concept IsIntEnumType = IsIntegralType<T> || IsEnumType<T>;

	template<typename T, typename... Args>
	concept IsSameType = std::conjunction_v<std::is_same<T, Args>...>;

	template<typename... Args>
	concept IsNonEmptyArgs = sizeof...(Args) > 0;

	template<std::size_t sz, typename... Args>
	concept IsSameSizeArgs = requires 
	{
		sz == (sizeof(Args) + ... + 0); 
	};

	template<typename T, typename... Args>
	concept IsPresentType = (std::same_as<T, Args> || ...);

	template<typename... Ts>
	concept HasNoDuplicateTypes = requires
	{
		tr::NoDuplicates<Ts...>{};
	};

	template<typename T>
	concept HasValueType = requires
	{
		typename T::value_type;
	};

	template<typename T>
	concept HasKeyType = requires
	{
		typename T::key_type;
	};

	template<typename T>
	concept HasMappedType = requires
	{
		typename T::mapped_type;
	};

	template<typename F, typename T>
	concept IsHashableType = std::regular_invocable<F, T> && requires(F f, T t) 
	{
		{ std::invoke(f, t) } -> std::convertible_to<size_t>;
	};

	template<class T, class... Args>
	concept ContainsType = std::disjunction_v<std::is_same<T, Args>...>;

	template<class Lambda, typename T, std::size_t... Index>
	concept SameTypeParamDecay = (std::same_as<T, std::decay_t<typename tr::FunctionTraits<Lambda>::template arg_type<Index>>> && ...);

	template<class Lambda, typename T, std::size_t... Index>
	concept SameTypeParam = (std::same_as<T, typename tr::FunctionTraits<Lambda>::template arg_type<Index>> && ...);

	template<class Lambda, typename... Ts>
	concept HasTypeParameters = (std::same_as<std::tuple<Ts...>, typename tr::FunctionTraits<Lambda>::arguments>);

	template<class Lambda, typename... Ts>
	concept HasTypeParametersDecay = (std::same_as<std::tuple<std::decay_t<Ts>...>, typename tr::FunctionTraits<Lambda>::arguments_decay>);
}