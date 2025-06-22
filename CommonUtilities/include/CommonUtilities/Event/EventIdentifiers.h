#pragma once

#include <atomic>

namespace CommonUtilities
{
	namespace evnt
	{
		using IDType	= std::uint32_t;
		using AtomicID	= std::atomic<IDType>;
	}
}