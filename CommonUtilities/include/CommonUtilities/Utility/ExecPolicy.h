#pragma once

#include <execution>

namespace CommonUtilities
{
	enum class Policy
	{
		Seq,
		Unseq,
		Par,
		ParUnseq
	};

	template<class F>
	inline auto ExecPolicy(F&& aFunc, Policy aPolicy)
	{
		switch (aPolicy)
		{
			case Policy::Seq:		return aFunc(std::execution::seq);
			case Policy::Unseq:		return aFunc(std::execution::unseq);
			case Policy::Par:		return aFunc(std::execution::par);
			case Policy::ParUnseq:	return aFunc(std::execution::par_unseq);
		}
	}
}