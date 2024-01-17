#pragma once

#include <unordered_map>
#include <stdexcept>

#include <CommonUtilities/Config.h>
#include <CommonUtilities/Utility/NonCopyable.h>

namespace CommonUtilities
{
	template<typename Bind, typename Reg>
	class Binds : private NonCopyable
	{
	public:
		NODISC constexpr Reg& operator[](const Bind& aBind);
		NODISC constexpr const Reg& operator[](const Bind& aBind) const;

		NODISC constexpr Reg& At(const Bind& aBind);
		NODISC constexpr const Reg& At(const Bind& aBind) const;

		///	Set the bind to match to button
		/// 
		constexpr void Set(const Bind& aBind, const Reg& aButton);

		///	Remove an existing bind
		/// 
		constexpr void Remove(const Bind& aBind);

		///	\return Whether bind is set to some button
		/// 
		constexpr bool IsSet(const Bind& aBind);

		NODISC constexpr bool GetEnabled() const noexcept;
		constexpr void SetEnabled(bool aFlag) noexcept;

	protected:
		constexpr Binds() = default; // only allow derived classes to construct bind
		constexpr ~Binds() = default;

		std::unordered_map<Bind, Reg> myBinds;
		bool myEnabled {true};
	};
	template<typename Bind, typename Reg>
	constexpr Reg& Binds<Bind, Reg>::operator[](const Bind& aBind)
	{
		return At(aBind);
	}
	template<typename Bind, typename Reg>
	constexpr const Reg& Binds<Bind, Reg>::operator[](const Bind& aBind) const
	{
		return At(aBind);
	}

	template<typename Bind, typename Reg>
	constexpr Reg& Binds<Bind, Reg>::At(const Bind& aBind)
	{
		return const_cast<Reg&>(std::as_const(*this).At(aBind));
	}
	template<typename Bind, typename Reg>
	constexpr const Reg& Binds<Bind, Reg>::At(const Bind& aBind) const
	{
		const auto it = myBinds.find(aBind);
		if (it == myBinds.end())
		{
			throw std::runtime_error("Bind could not be found");
		}

		return it->second;
	}

	template<typename Bind, typename Reg>
	constexpr void Binds<Bind, Reg>::Set(const Bind& aBind, const Reg& aButton)
	{
		myBinds[aBind] = aButton;
	}

	template<typename Bind, typename Reg>
	constexpr void Binds<Bind, Reg>::Remove(const Bind& aBind)
	{
		const auto it = myBinds.find(aBind);
		if (it == myBinds.end())
		{
			throw std::runtime_error("Bind could not be found");
		}

		myBinds.erase(it);
	}

	template<typename Bind, typename Reg>
	constexpr bool Binds<Bind, Reg>::IsSet(const Bind& aBind)
	{
		const auto it = myBinds.find(aBind);
		return it != myBinds.end();
	}

	template<typename Bind, typename Reg>
	constexpr bool Binds<Bind, Reg>::GetEnabled() const noexcept
	{
		return myEnabled;
	}
	template<typename Bind, typename Reg>
	constexpr void Binds<Bind, Reg>::SetEnabled(bool aFlag) noexcept
	{
		myEnabled = aFlag;
	}
}