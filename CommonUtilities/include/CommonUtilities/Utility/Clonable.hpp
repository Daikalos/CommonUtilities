#pragma once

#include <memory>

#include "CRTP.hpp"

namespace CommonUtilities
{
	template<class Base, class Derived, class BaseClone = Base>
	class Clonable : public Base, public CRTP<Derived>
	{
	public:
		using Base::Base;

		NODISC constexpr virtual std::unique_ptr<BaseClone> Clone() const
		{
			return std::make_unique<Derived>(this->Underlying());
		}
	};
}