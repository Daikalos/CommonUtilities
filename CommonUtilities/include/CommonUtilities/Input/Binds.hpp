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
		NODISC CONSTEXPR Reg& operator[](const Bind& aBind);
		NODISC CONSTEXPR const Reg& operator[](const Bind& aBind) const;

		NODISC CONSTEXPR Reg& At(const Bind& aBind);
		NODISC CONSTEXPR const Reg& At(const Bind& aBind) const;

		///	Set the bind to match to button
		/// 
		CONSTEXPR void Set(const Bind& aBind, const Reg& aButton);

		///	Remove an existing bind
		/// 
		CONSTEXPR void Remove(const Bind& aBind);

		///	\return Whether bind is set to some button
		/// 
		CONSTEXPR bool IsSet(const Bind& aBind);

		NODISC CONSTEXPR bool GetEnabled() const noexcept;
		CONSTEXPR void SetEnabled(bool aFlag) noexcept;

	protected:
		CONSTEXPR Binds(); // only allow derived classes to construct bind
		CONSTEXPR ~Binds();

		std::unordered_map<Bind, Reg> myBinds;
		bool myEnabled {true};
	};

	template<typename Bind, typename Reg>
	CONSTEXPR Binds<Bind, Reg>::Binds() = default;

	template<typename Bind, typename Reg>
	CONSTEXPR Binds<Bind, Reg>::~Binds() = default;

	template<typename Bind, typename Reg>
	CONSTEXPR Reg& Binds<Bind, Reg>::operator[](const Bind& aBind)
	{
		return At(aBind);
	}
	template<typename Bind, typename Reg>
	CONSTEXPR const Reg& Binds<Bind, Reg>::operator[](const Bind& aBind) const
	{
		return At(aBind);
	}

	template<typename Bind, typename Reg>
	CONSTEXPR Reg& Binds<Bind, Reg>::At(const Bind& aBind)
	{
		return const_cast<Reg&>(std::as_const(*this).At(aBind));
	}
	template<typename Bind, typename Reg>
	CONSTEXPR const Reg& Binds<Bind, Reg>::At(const Bind& aBind) const
	{
		const auto it = myBinds.find(aBind);
		if (it == myBinds.end())
		{
			throw std::runtime_error("Bind could not be found");
		}

		return it->second;
	}

	template<typename Bind, typename Reg>
	CONSTEXPR void Binds<Bind, Reg>::Set(const Bind& aBind, const Reg& aButton)
	{
		myBinds[aBind] = aButton;
	}

	template<typename Bind, typename Reg>
	CONSTEXPR void Binds<Bind, Reg>::Remove(const Bind& aBind)
	{
		const auto it = myBinds.find(aBind);
		if (it == myBinds.end())
		{
			throw std::runtime_error("Bind could not be found");
		}

		myBinds.erase(it);
	}

	template<typename Bind, typename Reg>
	CONSTEXPR bool Binds<Bind, Reg>::IsSet(const Bind& aBind)
	{
		const auto it = myBinds.find(aBind);
		return it != myBinds.end();
	}

	template<typename Bind, typename Reg>
	CONSTEXPR bool Binds<Bind, Reg>::GetEnabled() const noexcept
	{
		return myEnabled;
	}
	template<typename Bind, typename Reg>
	CONSTEXPR void Binds<Bind, Reg>::SetEnabled(bool aFlag) noexcept
	{
		myEnabled = aFlag;
	}
}