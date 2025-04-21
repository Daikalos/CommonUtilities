#pragma once

#include <execution>

namespace CommonUtilities
{
	enum class Policy
	{
		seq,
		unseq,
		par,
		par_unseq
	};

	template<class F>
	inline auto ExecPolicy(F&& aFunc, Policy aPolicy)
	{
		switch (aPolicy)
		{
			case Policy::seq:		return aFunc(std::execution::seq);
			case Policy::unseq:		return aFunc(std::execution::unseq);
			case Policy::par:		return aFunc(std::execution::par);
			case Policy::par_unseq:	return aFunc(std::execution::par_unseq);
		}
	}
}