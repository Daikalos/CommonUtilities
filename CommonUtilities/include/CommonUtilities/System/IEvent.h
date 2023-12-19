#pragma once

#include "EventIdentifiers.h"

namespace CommonUtilities
{
	class IEvent
	{
	public:
		virtual ~IEvent() = default;

		virtual evnt::IDType operator-=(evnt::IDType aHandlerID) = 0;
		virtual bool RemoveID(evnt::IDType aHandlerID) = 0;

		virtual std::size_t Count() const = 0;
		virtual bool IsEmpty() const = 0;

		virtual void Reserve(std::size_t aSize) = 0;
		virtual void Clear() = 0;
	};
}