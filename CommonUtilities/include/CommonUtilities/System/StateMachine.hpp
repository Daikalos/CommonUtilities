#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include <CommonUtilities/Utility/ContainerUtils.hpp>
#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/System/Timer.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	/// StateMachine is a finite state machine where there can only be one active state at a time. 
	/// States should be derived like this: "class Foo : public StateMachine<IDType, Hash>::State { }". 
	/// StateMachine is furthermore virtual so you may specialize it for your environment.
	/// 
	/// \param IDType: Type of the ID used to manage states
	/// \param Hash: Function that generates the hash for IDType
	/// 
	template<typename IDType = std::uint32_t, typename Hash = std::hash<IDType>> requires IsHashable<Hash, IDType>
	class StateMachine : private NonCopyable
	{
	public:
		class State
		{
		public:
			using Ptr = std::unique_ptr<State>;

			State(const IDType& aID, StateMachine& aStateMachine);
			virtual ~State() = default;

			NODISC const IDType& GetID() const noexcept;

			virtual void Enter() = 0;
			virtual void Update(Timer& aTimer) = 0;
			virtual void Exit() = 0;

		protected:
			NODISC auto GetMachine() const -> const StateMachine&;
			NODISC auto GetMachine() -> StateMachine&;

		private:
			IDType			myID;
			StateMachine*	myStateMachine;
		};

		StateMachine();
		virtual ~StateMachine() = default;

		NODISC auto GetCurrentState() const -> const State&;
		NODISC auto GetCurrentState() -> State&;

		NODISC auto GetState(const IDType& aStateID) const -> const State&;
		NODISC auto GetState(const IDType& aStateID) -> State&;

		virtual bool TransitionTo(const IDType& aStateID);

		template<std::derived_from<State> S, typename... Args>
			requires std::constructible_from<S, const IDType&, StateMachine&, Args...>
		void AddState(const IDType& aStateID, Args&&... someArgs)
		{
			myStates[aStateID] = std::make_unique<S>(aStateID, *this, std::forward<Args>(someArgs)...);
		}

		void Update(Timer& aTimer);

	protected:
		using StatePtr	= typename State::Ptr;
		using StateMap	= std::unordered_map<IDType, StatePtr, Hash>;

		StateMap	myStates;
		State*		myCurrentState;
	};

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline StateMachine<IDType, Hash>::State::State(const IDType& aID, StateMachine& aStateMachine)
		: myID(aID), myStateMachine(&aStateMachine)
	{

	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline const IDType& StateMachine<IDType, Hash>::State::GetID() const noexcept
	{
		return myID;
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::State::GetMachine() const -> const StateMachine&
	{
		return *myStateMachine;
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::State::GetMachine() -> StateMachine&
	{
		return *myStateMachine;
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline StateMachine<IDType, Hash>::StateMachine()
		: myStates(), myCurrentState(nullptr) { }

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::GetCurrentState() const -> const State&
	{
		assert(myCurrentState != nullptr && "No state currently set");
		return *myCurrentState;
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::GetCurrentState() -> State&
	{
		assert(myCurrentState != nullptr && "No state currently set");
		return *myCurrentState;
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::GetState(const IDType& aStateID) const -> const State&
	{
		const auto it = myStates.find(aStateID);
		return *it->second.get();
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::GetState(const IDType& aStateID) -> State&
	{
		return const_cast<State&>(std::as_const(*this).GetState(aStateID));
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline bool StateMachine<IDType, Hash>::TransitionTo(const IDType& aStateID)
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

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline void StateMachine<IDType, Hash>::Update(Timer& aTimer)
	{
		if (myCurrentState != nullptr)
		{
			myCurrentState->Update(aTimer);
		}
	}
}