#pragma once

#include <CommonUtilities/Utility/ExecPolicy.h>

namespace CommonUtilities
{
	template<typename C, typename Func>
	void ParallelFor(C&& aContainer, Func&& aFunc, std::size_t aSwitchToThreadsThreshold = 0)
	{
		const Policy threadLoop = (aContainer.size() >= aSwitchToThreadsThreshold ? Policy::ParUnseq : Policy::Unseq);

		ExecPolicy(
			[&aContainer, func = std::forward<Func>(aFunc)](auto& policy)
			{
				std::for_each(policy, aContainer.begin(), aContainer.end(), func);
			}, threadLoop);
	}

	template<typename C, typename Func>
	void ParallelSort(C&& aContainer, Func&& aFunc, std::size_t aSwitchToThreadsThreshold = 0)
	{
		const Policy threadLoop = (aContainer.size() >= aSwitchToThreadsThreshold ? Policy::ParUnseq : Policy::Unseq);

		ExecPolicy(
			[&aContainer, func = std::forward<Func>(aFunc)](auto& policy)
			{
				std::sort(policy, aContainer.begin(), aContainer.end(), func);
			}, threadLoop);
	}

	template<typename C, typename Func>
	void ParallelSort(C&& aContainer, std::size_t aSwitchToThreadsThreshold = 0)
	{
		const Policy threadLoop = (aContainer.size() >= aSwitchToThreadsThreshold ? Policy::ParUnseq : Policy::Unseq);

		ExecPolicy(
			[&aContainer](auto& policy)
			{
				std::sort(policy, aContainer.begin(), aContainer.end());
			}, threadLoop);
	}
}