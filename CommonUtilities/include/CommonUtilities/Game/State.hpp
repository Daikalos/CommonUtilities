#pragma once

#include <memory>
#include <functional>
#include <Windows.h>

#include <CommonUtilities/Config.h>
#include <CommonUtilities/System/Timer.h>

namespace CommonUtilities
{
	template<typename T, typename IDType>
	class StateStack;

	template<typename T, typename IDType = std::uint32_t>
	class State
	{
	public:
		using Ptr = std::unique_ptr<State>;
		using Func = std::function<Ptr()>;

		State(const IDType& aID, StateStack<T, IDType>& aStateStack, const T& aContext);
		virtual ~State() = default;

		NODISC const IDType& GetID() const noexcept;

		///	OnCreated is called when first constructed
		/// 
		virtual void OnCreate() {}

		///	OnActivate is called whenever the state is put as last in the stack
		/// 
		virtual void OnActivate() {}

		///	OnDeactivate is called whenever the state is longer the last in the stack
		/// 
		virtual void OnDeactivate() {}

		///	OnDestroy is called when the state is removed from the stack
		/// 
		virtual void OnDestroy() {}

		virtual bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam) = 0;

		virtual bool Init() = 0;
		virtual bool PreUpdate(Timer& aTimer)	{ return true; }
		virtual bool Update(Timer& aTimer) = 0;
		virtual bool FixedUpdate(Timer& aTimer)	{ return true; }
		virtual bool PostUpdate(Timer& aTimer)	{ return true; }

		virtual void Render() const = 0;

	protected:
		NODISC const StateStack<T, IDType>& GetStack() const;
		NODISC StateStack<T, IDType>& GetStack();

		NODISC const T& GetContext() const;
		NODISC T& GetContext();

	private:
		IDType					myID;
		StateStack<T, IDType>*	myStateStack;
		T						myContext;
	};

	template<typename T, typename IDType>
	inline State<T, IDType>::State(const IDType& aID, StateStack<T, IDType>& aStateStack, const T& aContext)
		: myID(aID), myStateStack(aStateStack), myContext(aContext)
	{

	}

	template<typename T, typename IDType>
	inline const IDType& State<T, IDType>::GetID() const noexcept
	{
		return myID;
	}

	template<typename T, typename IDType>
	inline const StateStack<T, IDType>& State<T, IDType>::GetStack() const
	{
		return *myStateStack;
	}
	template<typename T, typename IDType>
	inline StateStack<T, IDType>& State<T, IDType>::GetStack()
	{
		return *myStateStack;
	}

	template<typename T, typename IDType>
	inline const T& State<T, IDType>::GetContext() const
	{
		return *myContext;
	}
	template<typename T, typename IDType>
	inline T& State<T, IDType>::GetContext()
	{
		return *myContext;
	}
}