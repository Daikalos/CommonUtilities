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
	/// States should be derived like this: "class Foo : public StateMachine<T, IDType, Hash>::State { }". 
	/// StateMachine is furthermore virtual so you may specialize it for your needs.
	/// 
	/// \param T: Application context which usually contains pointers to important objects (e.g., input)
	/// \param IDType: Type of the ID used to manage states
	/// \param Hash: Function that generates the hash for IDType
	/// 
	template<typename T, typename IDType = std::uint32_t, typename Hash = std::hash<IDType>> 
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	class StateMachine : private NonCopyable
	{
	public:
		class State
		{
		public:
			using Ptr = std::unique_ptr<State>;

			State(const IDType& aID, StateMachine& aStateMachine, const T& aContext);
			virtual ~State() = default;

			NODISC const IDType& GetID() const noexcept;

			virtual void Enter() = 0;
			virtual void Update(Timer& aTimer) = 0;
			virtual void Exit() = 0;

		protected:
			NODISC auto GetMachine() const -> const StateMachine&;
			NODISC auto GetMachine() -> StateMachine&;

			NODISC auto GetContext() const -> const T&;
			NODISC auto GetContext() -> T&;

		private:
			IDType			myID;
			StateMachine*	myStateMachine;
			T				myContext;
		};

		StateMachine(const T& aContext = T());
		virtual ~StateMachine() = default;

		NODISC auto GetCurrentState() const -> const State&;
		NODISC auto GetCurrentState() -> State&;

		NODISC auto GetState(const IDType& aStateID) const -> const State&;
		NODISC auto GetState(const IDType& aStateID) -> State&;

		virtual bool TransitionTo(const IDType& aStateID);

		template<std::derived_from<State> S, typename... Args>
			requires std::constructible_from<S, const IDType&, StateMachine&, const T&, Args...>
		void AddState(const IDType& aStateID, Args&&... someArgs)
		{
			myStates[aStateID] = std::make_unique<S>(aStateID, *this, myContext, std::forward<Args>(someArgs)...);
		}

		void Update(Timer& aTimer);

	protected:
		using StatePtr	= typename State::Ptr;
		using StateMap	= std::unordered_map<IDType, StatePtr, Hash>;

		T			myContext;
		StateMap	myStates;
		State*		myCurrentState;
	};

	template<typename T, typename IDType, typename Hash> 
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline StateMachine<T, IDType, Hash>::State::State(const IDType& aID, StateMachine& aStateMachine, const T& aContext)
		: myID(aID), myStateMachine(&aStateMachine), myContext(aContext)
	{

	}

	template<typename T, typename IDType, typename Hash> 
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline const IDType& StateMachine<T, IDType, Hash>::State::GetID() const noexcept
	{
		return myID;
	}

	template<typename T, typename IDType, typename Hash> 
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline auto StateMachine<T, IDType, Hash>::State::GetMachine() const -> const StateMachine&
	{
		return *myStateMachine;
	}
	template<typename T, typename IDType, typename Hash> 
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline auto StateMachine<T, IDType, Hash>::State::GetMachine() -> StateMachine&
	{
		return *myStateMachine;
	}

	template<typename T, typename IDType, typename Hash> 
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline auto StateMachine<T, IDType, Hash>::State::GetContext() const -> const T&
	{
		return myContext;
	}
	template<typename T, typename IDType, typename Hash> 
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline auto StateMachine<T, IDType, Hash>::State::GetContext() -> T&
	{
		return myContext;
	}

	template<typename T, typename IDType, typename Hash> 
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline StateMachine<T, IDType, Hash>::StateMachine(const T& aContext)
		: myContext(aContext), myStates(), myCurrentState(nullptr) { }

	template<typename T, typename IDType, typename Hash> 
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline auto StateMachine<T, IDType, Hash>::GetCurrentState() const -> const State&
	{
		assert(myCurrentState != nullptr && "No state currently set");
		return *myCurrentState;
	}
	template<typename T, typename IDType, typename Hash> 
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline auto StateMachine<T, IDType, Hash>::GetCurrentState() -> State&
	{
		assert(myCurrentState != nullptr && "No state currently set");
		return *myCurrentState;
	}
	template<typename T, typename IDType, typename Hash> 
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline auto StateMachine<T, IDType, Hash>::GetState(const IDType& aStateID) const -> const State&
	{
		const auto it = myStates.find(aStateID);
		return *it->second.get();
	}
	template<typename T, typename IDType, typename Hash> 
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline auto StateMachine<T, IDType, Hash>::GetState(const IDType& aStateID) -> State&
	{
		return const_cast<State&>(std::as_const(*this).GetState(aStateID));
	}

	template<typename T, typename IDType, typename Hash>
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline bool StateMachine<T, IDType, Hash>::TransitionTo(const IDType& aStateID)
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

	template<typename T, typename IDType, typename Hash>
		requires std::is_default_constructible_v<T> && IsHashable<Hash, IDType>
	inline void StateMachine<T, IDType, Hash>::Update(Timer& aTimer)
	{
		if (myCurrentState != nullptr)
		{
			myCurrentState->Update(aTimer);
		}
	}
}