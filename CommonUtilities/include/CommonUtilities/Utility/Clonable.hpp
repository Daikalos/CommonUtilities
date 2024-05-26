#pragma once

#include <memory>

#include "CRTP.hpp"

namespace CommonUtilities
{
	template<class Base, class Derived>
	class Clonable : public Base, public CRTP<Derived>
	{
	public:
		using Base::Base;

		NODISC constexpr virtual std::unique_ptr<Base> Clone() const;
	};

	template<class Base, class Derived>
	constexpr std::unique_ptr<Base> Clonable<Base, Derived>::Clone() const
	{
		return std::make_unique<Derived>(this->Underlying());
	}
}