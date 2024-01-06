#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <Windows.h>

#include <CommonUtilities/Utility/ContainerUtils.hpp>
#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/System/Timer.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	/// StateMachine is a finite state machine where there can only be one active state at a time. 
	/// States should be derived like this: "class Foo : public StateMachine<T, IDType>::State { }".
	/// 
	/// \param T: Application context which usually contains pointers to important objects (e.g., input)
	/// \param IDType: Type of the ID used to manage states
	/// 
	template<typename T, typename IDType = std::uint32_t>
	class StateMachine : private NonCopyable
	{
	public:
		class State
		{
		public:
			using Ptr = std::unique_ptr<State>;
			using Context = std::remove_const_t<std::remove_reference_t<T>>;

			State(const IDType& aID, StateMachine& aStateMachine, const Context& aContext);
			virtual ~State() = default;

			NODISC const IDType& GetID() const noexcept;

			virtual void Enter() = 0;
			virtual void PreUpdate([[maybe_unused]] Timer& aTimer) {}
			virtual void Update(Timer& aTimer) = 0;
			virtual void FixedUpdate([[maybe_unused]] Timer& aTimer) {}
			virtual void PostUpdate([[maybe_unused]] Timer& aTimer) {}
			virtual void Exit() = 0;

		protected:
			NODISC auto GetMachine() const -> const StateMachine&;
			NODISC auto GetMachine() -> StateMachine&;

			NODISC auto GetContext() const -> const Context&;
			NODISC auto GetContext() -> Context&;

		private:
			IDType			myID;
			StateMachine*	myStateMachine;
			Context			myContext;
		};

		StateMachine(const typename State::Context& aContext);

		NODISC auto GetCurrentState() const -> const State&;
		NODISC auto GetCurrentState() -> State&;

		NODISC auto GetState(const IDType& aStateID) const -> const State&;
		NODISC auto GetState(const IDType& aStateID) -> State&;

		bool TransitionTo(const IDType& aStateID);

		template<std::derived_from<State> S, typename... Args>
			requires std::constructible_from<S, const IDType&, StateMachine&, const typename State::Context&, Args...>
		void AddState(const IDType& aStateID, Args&&... someArgs)
		{
			myStates[aStateID] = std::make_unique<S>(aStateID, *this, myContext, std::forward<Args>(someArgs)...);
		}

		void PreUpdate(Timer& aTimer);
		void Update(Timer& aTimer);
		void FixedUpdate(Timer& aTimer);
		void PostUpdate(Timer& aTimer);

	private:
		using Context = typename State::Context;
		using StatePtr = typename State::Ptr;
		using StateMap = std::unordered_map<IDType, StatePtr>;

		Context		myContext;
		StateMap	myStates;
		State*		myCurrentState;
	};

	template<typename T, typename IDType>
	inline StateMachine<T, IDType>::State::State(const IDType& aID, StateMachine& aStateMachine, const Context& aContext)
		: myID(aID), myStateMachine(&aStateMachine), myContext(aContext)
	{

	}

	template<typename T, typename IDType>
	inline const IDType& StateMachine<T, IDType>::State::GetID() const noexcept
	{
		return myID;
	}

	template<typename T, typename IDType>
	inline auto StateMachine<T, IDType>::State::GetMachine() const -> const StateMachine&
	{
		return *myStateMachine;
	}
	template<typename T, typename IDType>
	inline auto StateMachine<T, IDType>::State::GetMachine() -> StateMachine&
	{
		return *myStateMachine;
	}

	template<typename T, typename IDType>
	inline auto StateMachine<T, IDType>::State::GetContext() const -> const Context&
	{
		return myContext;
	}
	template<typename T, typename IDType>
	inline auto StateMachine<T, IDType>::State::GetContext() -> Context&
	{
		return myContext;
	}

	template<typename T, typename IDType>
	inline StateMachine<T, IDType>::StateMachine(const typename State::Context& aContext)
		: myContext(aContext) { }
	template<typename T, typename IDType>
	inline auto StateMachine<T, IDType>::GetCurrentState() const -> const State&
	{
		assert(myCurrentState != nullptr && "No state currently set");
		return *myCurrentState;
	}
	template<typename T, typename IDType>
	inline auto StateMachine<T, IDType>::GetCurrentState() -> State&
	{
		assert(myCurrentState != nullptr && "No state currently set");
		return *myCurrentState;
	}
	template<typename T, typename IDType>
	inline auto StateMachine<T, IDType>::GetState(const IDType& aStateID) const -> const State&
	{
		const auto it = myStates.find(aStateID);
		return *it->second.get();
	}
	template<typename T, typename IDType>
	inline auto StateMachine<T, IDType>::GetState(const IDType& aStateID) -> State&
	{
		return const_cast<State&>(std::as_const(*this).GetState(aStateID));
	}

	template<typename T, typename IDType>
	inline bool StateMachine<T, IDType>::TransitionTo(const IDType& aStateID)
	{
		const auto it = myStates.find(aStateID);
		if (it == myStates.end())
		{
			return false;
		}

		State* newState = it->second.get();
		if (newState == myCurrentState)
		{
			return false;
		}

		if (myCurrentState != nullptr)
		{
			myCurrentState->Exit();
		}

		myCurrentState = newState;

		if (myCurrentState != nullptr)
		{
			myCurrentState->Enter();
		}

		return true;
	}

	template<typename T, typename IDType>
	inline void StateMachine<T, IDType>::PreUpdate(Timer& aTimer)
	{
		if (myCurrentState != nullptr)
		{
			myCurrentState->PreUpdate(aTimer);
		}
	}
	template<typename T, typename IDType>
	inline void StateMachine<T, IDType>::Update(Timer& aTimer)
	{
		if (myCurrentState != nullptr)
		{
			myCurrentState->Update(aTimer);
		}
	}
	template<typename T, typename IDType>
	inline void StateMachine<T, IDType>::FixedUpdate(Timer& aTimer)
	{
		if (myCurrentState != nullptr)
		{
			myCurrentState->FixedUpdate(aTimer);
		}
	}
	template<typename T, typename IDType>
	inline void StateMachine<T, IDType>::PostUpdate(Timer& aTimer)
	{
		if (myCurrentState != nullptr)
		{
			myCurrentState->PostUpdate(aTimer);
		}
	}
}