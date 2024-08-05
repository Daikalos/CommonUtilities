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
	template<typename IDType = std::uint32_t, typename Hash = std::hash<IDType>> requires IsHashableType<Hash, IDType>
	class StateMachine
	{
	public:
		class State
		{
		public:
			using Ptr = std::unique_ptr<State>;

			State(const IDType& aID, StateMachine& aStateMachine);
			virtual ~State() = default;

			NODISC const IDType& GetID() const noexcept;

			void SetMachine(StateMachine& aStateMachine) noexcept;

			virtual std::unique_ptr<State> Clone() const = 0;

			virtual void Init() {};
			virtual void Enter() {};
			virtual void Exit() {};

			virtual void Update(Timer& aTimer) = 0;

		protected:
			NODISC virtual auto GetMachine() const -> const StateMachine&;
			NODISC virtual auto GetMachine() -> StateMachine&;

		private:
			IDType			myID;
			StateMachine*	myStateMachine;
		};

		StateMachine() = default;
		virtual ~StateMachine() = default;

		StateMachine(StateMachine&& aOther) noexcept;
		StateMachine(const StateMachine& aOther);

		StateMachine& operator=(StateMachine&& aOther) noexcept;
		StateMachine& operator=(const StateMachine& aOther);

		NODISC virtual auto GetCurrentState() const -> const State&;
		NODISC virtual auto GetCurrentState() -> State&;

		NODISC virtual auto GetPreviousState() const -> const State&;
		NODISC virtual auto GetPreviousState() -> State&;

		NODISC virtual auto GetState(const IDType& aStateID) const -> const State&;
		NODISC virtual auto GetState(const IDType& aStateID) -> State&;

		/// Transitions from the current state to the new one.
		/// 
		/// \param StateID: ID of the state to transition to.
		/// 
		/// \return Whether it transitioned successfully
		/// 
		virtual bool TransitionTo(const IDType& aStateID);

		/// Exits the current state and sets the currently active one to null.
		/// 
		void Stop();

		/// Adds the state directly to the machine so that you can later transition from and to it.
		/// 
		/// \param StateID: ID of the state
		/// \param Args: Optional constructor arguments
		/// 
		template<std::derived_from<State> S, typename... Args>
			requires std::constructible_from<S, const IDType&, StateMachine&, Args...>
		void AddState(const IDType& aStateID, Args&&... someArgs)
		{
			auto statePtr = std::make_unique<S>(aStateID, *this, std::forward<Args>(someArgs)...);
			statePtr->Init();

			myStates[aStateID] = std::move(statePtr);
		}

		/// Removes the state from the machine.
		/// 
		/// \param StateID: ID of the state to remove
		/// 
		/// \return Whether it successfully removed the state
		/// 
		bool RemoveState(const IDType& aStateID);

		void Clear();

		/// Generic update function that calls update on the currently active state.
		/// 
		virtual void Update(Timer& aTimer);

	protected:
		using StatePtr	= typename State::Ptr;
		using StateMap	= std::unordered_map<IDType, StatePtr, Hash>;

		StateMap	myStates;
		State*		myCurrentState	{nullptr};
		State*		myPreviousState {nullptr};

	private:
		void DeepCopy(const StateMachine& aOther);
	};

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline StateMachine<IDType, Hash>::State::State(const IDType& aID, StateMachine& aStateMachine)
		: myID(aID), myStateMachine(&aStateMachine)
	{

	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline const IDType& StateMachine<IDType, Hash>::State::GetID() const noexcept
	{
		return myID;
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void StateMachine<IDType, Hash>::State::SetMachine(StateMachine& aStateMachine) noexcept
	{
		myStateMachine = &aStateMachine;
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::State::GetMachine() const -> const StateMachine&
	{
		return *myStateMachine;
	}
	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::State::GetMachine() -> StateMachine&
	{
		return *myStateMachine;
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline StateMachine<IDType, Hash>::StateMachine(StateMachine&& aOther) noexcept
		: myStates(std::move(aOther.myStates))
		, myCurrentState(std::exchange(aOther.myCurrentState, nullptr))
		, myPreviousState(std::exchange(aOther.myPreviousState, nullptr))
	{
		for (auto& [id, statePtr] : myStates)
		{
			if (statePtr != nullptr)
			{
				statePtr->SetMachine(*this); // update references to new machine
			}
		}
	}
	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline StateMachine<IDType, Hash>::StateMachine(const StateMachine& aOther)
	{
		if (this == &aOther)
			return;

		DeepCopy(aOther);
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline StateMachine<IDType, Hash>& StateMachine<IDType, Hash>::operator=(StateMachine&& aOther) noexcept
	{
		if (this == &aOther)
			return *this;

		myStates		= std::move(aOther.myStates);
		myCurrentState	= std::exchange(aOther.myCurrentState, nullptr);
		myPreviousState = std::exchange(aOther.myPreviousState, nullptr);

		for (auto& [id, statePtr] : myStates)
		{
			if (statePtr != nullptr)
			{
				statePtr->SetMachine(*this);
			}
		}

		return *this;
	}
	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline StateMachine<IDType, Hash>& StateMachine<IDType, Hash>::operator=(const StateMachine& aOther)
	{
		if (this == &aOther)
			return *this;

		DeepCopy(aOther);

		return *this;
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::GetCurrentState() const -> const State&
	{
		assert(myCurrentState != nullptr && "No state currently set");
		return *myCurrentState;
	}
	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::GetCurrentState() -> State&
	{
		assert(myCurrentState != nullptr && "No state currently set");
		return *myCurrentState;
	}
	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::GetPreviousState() const -> const State&
	{
		assert(myPreviousState != nullptr && "No state currently set");
		return *myPreviousState;
	}
	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::GetPreviousState() -> State&
	{
		assert(myPreviousState != nullptr && "No state currently set");
		return *myPreviousState;
	}
	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::GetState(const IDType& aStateID) const -> const State&
	{
		const auto it = myStates.find(aStateID);
		return *it->second.get();
	}
	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateMachine<IDType, Hash>::GetState(const IDType& aStateID) -> State&
	{
		return const_cast<State&>(std::as_const(*this).GetState(aStateID));
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
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

		myPreviousState = (myCurrentState != nullptr) ? myCurrentState : newState;
		myCurrentState = newState;

		if (myCurrentState != nullptr)
		{
			myCurrentState->Enter();
		}

		return true;
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void StateMachine<IDType, Hash>::Stop()
	{
		if (myCurrentState != nullptr)
		{
			myCurrentState->Exit();
		}

		myCurrentState = nullptr;
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline bool StateMachine<IDType, Hash>::RemoveState(const IDType& aStateID)
	{
		const auto it = myStates.find(aStateID);
		if (it == myStates.end())
		{
			return false;
		}

		myStates.erase(it);

		return true;
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void StateMachine<IDType, Hash>::Clear()
	{
		myStates.clear();
		myCurrentState = nullptr;
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void StateMachine<IDType, Hash>::Update(Timer& aTimer)
	{
		if (myCurrentState != nullptr)
		{
			myCurrentState->Update(aTimer);
		}
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void StateMachine<IDType, Hash>::DeepCopy(const StateMachine& aOther)
	{
		myStates.clear();

		myCurrentState = nullptr;
		myPreviousState = nullptr;

		for (auto& [id, ptr] : aOther.myStates)
		{
			if (ptr != nullptr)
			{
				auto statePtr = ptr->Clone();

				statePtr->SetMachine(*this);
				statePtr->Init();

				myStates[id] = std::move(statePtr);
			}
		}

		TransitionTo(aOther.GetCurrentState().GetID());
	}
}