#pragma once

#include <memory>
#include <functional>
#include <Windows.h>

#include <CommonUtilities/Config.h>
#include <CommonUtilities/System/Timer.h>

#include "StateIdentifiers.h"

namespace CommonUtilities
{
	template<typename T>
	class StateStack;

	template<typename T>
	class State
	{
	public:
		using Ptr = std::unique_ptr<State>;
		using Func = std::function<Ptr()>;

		State(StateID aID, StateStack<T>& aStateStack, T& aContext);
		virtual ~State() = default;

		NODISC StateID GetID() const noexcept;

		virtual void OnCreate() {}
		virtual void OnActivate() {}
		virtual void OnDestroy() {}

		virtual bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam) = 0;

		virtual bool Init(Timer& aTimer) = 0;
		virtual bool PreUpdate(Timer& aTimer)	{ return true; }
		virtual bool Update(Timer& aTimer) = 0;
		virtual bool FixedUpdate(Timer& aTimer)	{ return true; }
		virtual bool PostUpdate(Timer& aTimer)	{ return true; }

		virtual void Render() const = 0;

	protected:
		NODISC const StateStack<T>& GetStack() const;
		NODISC StateStack<T>& GetStack();

		NODISC const T& GetContext() const;
		NODISC T& GetContext();

	private:
		StateID myID;
		StateStack<T>* myStateStack;
		T* myContext;
	};

	template<typename T>
	inline State<T>::State(StateID aID, StateStack<T>& aStateStack, T& aContext)
		: myID(aID), myStateStack(aStateStack), myContext(aContext)
	{

	}

	template<typename T>
	inline StateID State<T>::GetID() const noexcept
	{
		return myID;
	}

	template<typename T>
	inline const StateStack<T>& State<T>::GetStack() const
	{
		return *myStateStack;
	}
	template<typename T>
	inline StateStack<T>& State<T>::GetStack()
	{
		return *myStateStack;
	}

	template<typename T>
	inline const T& State<T>::GetContext() const
	{
		return *myContext;
	}
	template<typename T>
	inline T& State<T>::GetContext()
	{
		return *myContext;
	}
}