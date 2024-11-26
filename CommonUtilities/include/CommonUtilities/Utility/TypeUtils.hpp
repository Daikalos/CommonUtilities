#pragma once

#include <string>
#include <string_view>

#if __cplusplus >= 202002L
#include <source_location>
#endif

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template <typename T> 
	constexpr std::string_view TypeName();

	template <>
	constexpr std::string_view TypeName<void>()
	{
		return "void"; // use void as probe to find position of the type in the signature
	}

	template <typename T>
	constexpr std::string_view WrappedTypeName() 
	{
#if __cplusplus >= 202002L
		return std::source_location::current().function_name();
#elif defined(COMMON_UTILITIES_PRETTY_FUNCTION)
		return COMMON_UTILITIES_PRETTY_FUNCTION;
#else
#	error "Unsupported compiler"
#endif
	}

	namespace details
	{
		using TypeNameProber = void;

		constexpr std::size_t WrappedTypeNamePrefixLength()
		{
			return WrappedTypeName<TypeNameProber>().find(TypeName<TypeNameProber>());
		}

		constexpr std::size_t WrappedTypeNameSuffixLength()
		{
			return WrappedTypeName<TypeNameProber>().length()
				- WrappedTypeNamePrefixLength()
				- TypeName<TypeNameProber>().length();
		}
	}

	template <typename T>
	constexpr std::string_view TypeName()
	{
		constexpr auto wrappedName	= WrappedTypeName<T>();
		constexpr auto prefixLength = details::WrappedTypeNamePrefixLength();
		constexpr auto suffixLength = details::WrappedTypeNameSuffixLength();

		constexpr auto typeNameLength = wrappedName.length() - prefixLength - suffixLength;

		return wrappedName.substr(prefixLength, typeNameLength);
	}

	template <typename T>
	constexpr std::string_view TypeNameClean()
	{
		constexpr auto typeName = cu::TypeName<T>();
		constexpr auto index	= typeName.find_last_of(' ') + 1;

		return typeName.substr(index);
	}
}