#pragma once

#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Config.h>

#include "State.hpp"

namespace CommonUtilities
{
	template<typename T>
	class StateStack : private NonCopyable
	{
	public:


		enum class Action
		{
			Push,
			Pop,
			Erase,
			Clear
		};

	private:

	};
}