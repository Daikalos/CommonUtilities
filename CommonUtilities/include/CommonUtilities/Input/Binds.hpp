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
		NODISC bool IsEnabled() const noexcept;

		void SetEnabled(bool aFlag);

		///	Set the bind to match to button
		/// 
		void Set(const Bind& aBind, const Reg& aButton);

		///	Remove an existing bind
		/// 
		void Remove(const Bind& aBind);

		///	\return Whether bind is set to some button
		/// 
		bool IsSet(const Bind& aBind) const;

	protected:
		Binds() = default; // only allow derived classes to construct bind
		~Binds() = default;

		NODISC auto operator[](const Bind& aBind) const;
		NODISC auto operator[](const Bind& aBind);

		NODISC auto At(const Bind& aBind) const;
		NODISC auto At(const Bind& aBind);

		std::unordered_multimap<Bind, Reg> myBinds;
		bool myEnabled {true};
	};

	template<typename Bind, typename Reg>
	inline bool Binds<Bind, Reg>::IsEnabled() const noexcept
	{
		return myEnabled;
	}

	template<typename Bind, typename Reg>
	inline void Binds<Bind, Reg>::SetEnabled(bool aFlag)
	{
		myEnabled = aFlag;
	}

	template<typename Bind, typename Reg>
	inline void Binds<Bind, Reg>::Set(const Bind& aBind, const Reg& aButton)
	{
		myBinds.insert(std::make_pair(aBind, aButton));
	}

	template<typename Bind, typename Reg>
	inline void Binds<Bind, Reg>::Remove(const Bind& aBind)
	{
		const auto it = myBinds.find(aBind);
		if (it == myBinds.end())
		{
			throw std::runtime_error("Bind could not be found");
		}

		myBinds.erase(it);
	}

	template<typename Bind, typename Reg>
	inline bool Binds<Bind, Reg>::IsSet(const Bind& aBind) const
	{
		const auto it = myBinds.find(aBind);
		return it != myBinds.end();
	}

	template<typename Bind, typename Reg>
	inline auto Binds<Bind, Reg>::operator[](const Bind& aBind) const
	{
		return At(aBind);
	}
	template<typename Bind, typename Reg>
	inline auto Binds<Bind, Reg>::operator[](const Bind& aBind)
	{
		return At(aBind);
	}

	template<typename Bind, typename Reg>
	inline auto Binds<Bind, Reg>::At(const Bind& aBind) const
	{
		auto range = myBinds.equal_range(aBind);

		if (range.first == myBinds.end() && range.second == myBinds.end())
		{
			throw std::runtime_error("Bind could not be found");
		}

		return range;
	}
	template<typename Bind, typename Reg>
	inline auto Binds<Bind, Reg>::At(const Bind& aBind)
	{
		auto range = myBinds.equal_range(aBind);

		if (range.first == myBinds.end() && range.second == myBinds.end())
		{
			throw std::runtime_error("Bind could not be found");
		}

		return range;
	}
}