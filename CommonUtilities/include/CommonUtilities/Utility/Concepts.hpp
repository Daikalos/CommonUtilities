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
	concept IsArithmetic = std::is_arithmetic_v<T>;

	template<typename T>
	concept IsIntegral = std::is_integral_v<T>;

	template<typename T>
	concept IsFloatingPoint = std::is_floating_point_v<T>;

	template<typename T>
	concept IsEnum = std::is_enum_v<T>;

	template<typename T>
	concept IsArithEnum = IsArithmetic<T> || IsEnum<T>;

	template<typename T>
	concept IsIntEnum = IsIntegral<T> || IsEnum<T>;

	template<typename T, typename... Args>
	concept IsSameType = std::conjunction_v<std::is_same<T, Args>...>;

	template<typename... Args>
	concept IsNonEmpty = sizeof...(Args) > 0;

	template<std::size_t sz, typename... Args>
	concept IsSameSize = requires 
	{
		sz == (sizeof(Args) + ... + 0); 
	};

	template<typename T, typename... Args>
	concept IsPresent = (std::same_as<T, Args> || ...);

	template<typename... Ts>
	concept HasNoDuplicates = requires
	{
		Traits::NoDuplicates<Ts...>{};
	};

	template<typename F, typename T>
	concept IsHashable = std::regular_invocable<F, T> && requires(F f, T t) 
	{
		{ std::invoke(f, t) } -> std::convertible_to<size_t>;
	};

	template<class T, class... Args>
	concept Contains = std::disjunction_v<std::is_same<T, Args>...>;

	template<class Lambda, typename T, std::size_t... Index>
	concept SameTypeParamDecay = (std::same_as<T, std::decay_t<typename Traits::FunctionTraits<Lambda>::template arg_type<Index>>> && ...);

	template<class Lambda, typename T, std::size_t... Index>
	concept SameTypeParam = (std::same_as<T, typename Traits::FunctionTraits<Lambda>::template arg_type<Index>> && ...);

	template<class Lambda, typename... Ts>
	concept HasParameters = (std::same_as<std::tuple<Ts...>, typename Traits::FunctionTraits<Lambda>::arguments>);

	template<class Lambda, typename... Ts>
	concept HasParametersDecay = (std::same_as<std::tuple<std::decay_t<Ts>...>, typename Traits::FunctionTraits<Lambda>::arguments_decay>);
}