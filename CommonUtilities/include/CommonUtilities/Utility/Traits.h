#pragma once

#include <type_traits>

#include <CommonUtilities/Config.h>

#if !C20_SUPPORT
#error Some traits here use C++20 features; please change the language standard or don't include the header that caused this message.
#endif

namespace CommonUtilities::Traits
{
    template <typename T>
    struct FunctionTraits : public FunctionTraits<decltype(&std::remove_reference_t<T>::operator())> {};

    template <typename ReturnType, typename... Args>
    struct FunctionTraits<ReturnType(*)(Args...)> : FunctionTraits<ReturnType(Args...)> {};

    template <typename ReturnType, typename ClassType, typename... Args>
    struct FunctionTraits<ReturnType(ClassType::*)(Args...) const> : FunctionTraits<ReturnType(ClassType::*)(Args...)> {};

    template <typename ReturnType, typename ClassType, typename... Args>
    struct FunctionTraits<ReturnType(ClassType::*)(Args...)> : FunctionTraits<ReturnType(Args...)> {};

    template <typename ReturnType, typename... Args>
    struct FunctionTraits<ReturnType(Args...)>
    {
        using arguments = std::tuple<Args...>;
        using arguments_decay = std::tuple<std::decay_t<Args>...>;

        static constexpr std::size_t arity = std::tuple_size_v<arguments>;
        
        using result_type = ReturnType;

        template <std::size_t Index>
        using arg_type = std::tuple_element_t<Index, std::tuple<Args...>>;
    };

    template<typename T>
    struct Base {};

    template<typename... Ts> 
    struct NoDuplicates : Base<Ts>... // can't inherit from the same base multiple times, therefore error
    {
        constexpr operator bool() const { return true; }
    };

    namespace priv
    {
        template<std::size_t Index, typename T, typename Tuple> requires (Index < std::tuple_size_v<Tuple>)
        static constexpr std::size_t IndexInTupleFn()
        {
            if constexpr (std::is_same_v<T, std::tuple_element_t<Index, Tuple>>)
            {
                return Index;
            }
            else
            {
                return IndexInTupleFn<Index + 1, T, Tuple>();
            }
        }
    }

    /// Returns in compile-time the index to where in tuple the type is located at
    /// 
    template<typename T, typename Tuple>
    struct IndexInTuple
    {
        static constexpr std::size_t value = priv::IndexInTupleFn<0, T, Tuple>();
    };
}
