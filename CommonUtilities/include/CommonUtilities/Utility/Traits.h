#pragma once

#include <type_traits>

#include <CommonUtilities/Config.h>

#if !C20_SUPPORT
#error Some traits here use C++20 features; please change the language standard or don't include the header that caused this message.
#endif

namespace CommonUtilities::tr
{
    namespace details
    {
        template<std::size_t Index, typename T, typename Tuple> requires (Index < std::tuple_size_v<Tuple>)
        static constexpr std::size_t IndexInTupleImpl()
        {
            if constexpr (std::is_same_v<T, std::tuple_element_t<Index, Tuple>>)
            {
                return Index;
            }
            else
            {
                return IndexInTupleImpl<Index + 1, T, Tuple>();
            }
        }
    }

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
    struct ClassTag {};

    template<typename T>
    struct Base {};

    template<typename... Ts> 
    struct NoDuplicates : Base<Ts>... // can't inherit from the same base multiple times, therefore error
    {
        constexpr operator bool() const { return true; }
    };

    /// Allows for you to, e.g., visit the item in a variant through its active type.
    /// 
    template<typename... Ts>
    struct Overload : Ts...
    {
        using Ts::operator() ...;
    };

    template<class T> 
    struct DependentFalse : std::false_type {};

#ifndef C20_SUPPORT
    /// Deduction guide for compiler on how to specify argument template parameters.
    /// 
    template<class... Ts> Overload(Ts...) -> Overload<Ts...>;
#endif

    /// Returns in compile-time the index to where in tuple the type is located at.
    /// 
    template<typename T, typename Tuple>
    struct IndexInTuple
    {
        static constexpr std::size_t value = details::IndexInTupleImpl<0, T, Tuple>();
    };
}
