#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <cassert>

#include <CommonUtilities/Utility/ContainerUtils.hpp>
#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/System/Timer.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	/// StateStack is a simple container for states that are managed similarly to a stack. 
	/// States should be derived like this: "class Foo : public StateStack<T, IDType, Hash>::State { }".
	/// StateStack is furthermore virtual so you may specialize it for your environment.
	/// 
	/// \param T: Application context that usually contain pointers to important objects (e.g. pointer to input)
	/// \param IDType: Type of the ID used to manage states
	/// \param Hash: Function that generates the hash for IDType
	/// 
	template<typename T, typename IDType = std::uint32_t, typename Hash = std::hash<IDType>> requires IsHashableType<Hash, IDType>
	class StateStack : private NonCopyable
	{
	public:
		class State
		{
		public:
			using Ptr	= std::unique_ptr<State>;
			using Func	= std::function<Ptr()>;

			State(const T& aContext, const IDType& aID, StateStack& aStateStack);
			virtual ~State() = default;

			/// \return ID of the current state
			/// 
			NODISC const IDType& GetID() const noexcept;

			///	OnCreated is called when first constructed.
			/// 
			virtual void OnCreate() {}

			///	OnActivate is called whenever the state is put as last in the stack.
			/// 
			virtual void OnActivate() {}

			///	OnDeactivate is called whenever the state is longer the last in the stack.
			/// 
			virtual void OnDeactivate() {}

			///	OnDestroy is called when the state is removed from the stack.
			/// 
			virtual void OnDestroy() {}

			virtual bool Update(Timer& aTimer) = 0;

		protected:
			NODISC virtual auto GetContext() const -> const T&;
			NODISC virtual auto GetContext() -> T&;

			NODISC virtual auto GetStack() const -> const StateStack&;
			NODISC virtual auto GetStack() -> StateStack&;

		private:
			T			myContext;
			IDType		myID;
			StateStack*	myStateStack;
		};

		StateStack(const T& aContext);
		virtual ~StateStack() = default;

		NODISC virtual auto operator[](std::size_t aIndex) const -> const State&;
		NODISC virtual auto operator[](std::size_t aIndex) -> State&;

		NODISC virtual auto GetState(std::size_t aIndex) const -> const State&;
		NODISC virtual auto GetState(std::size_t aIndex) -> State&;

		NODISC virtual auto GetStateByID(const IDType& aStateID) const -> const State*;
		NODISC virtual auto GetStateByID(const IDType& aStateID) -> State*;

		NODISC virtual auto GetTopState() const -> const State&;
		NODISC virtual auto GetTopState() -> State&;

		NODISC virtual auto GetBotState() const -> const State&;
		NODISC virtual auto GetBotState() -> State&;

		/// \return Number of states currently in the stack
		/// 
		NODISC std::size_t Count() const noexcept;

		/// \return If the stack is currently empty
		/// 
		NODISC bool IsEmpty() const noexcept;

		/// Generic update function that calls update on all states and then applies all pending changes.
		/// 
		virtual void Update(Timer& aTimer);

		/// Push a state to the top of the stack.
		/// 
		/// \param StateID: ID of the state to add
		/// 
		void Push(const IDType& aStateID);

		/// Erases the first instance found with the provided ID.
		/// 
		/// \param StateID: ID of the state to erase
		/// 
		void Erase(const IDType& aStateID);

		/// Moves the first instance found with the provided ID to the new position.
		/// 
		/// \param StateID: ID of the state to erase
		/// \param NewIndex: The new position of the state
		/// 
		void Move(const IDType& aStateID, std::size_t aNewIndex);

		/// Pops the state currently at the top of the stack.
		/// 
		void Pop();

		/// Clears the stack.
		/// 
		void Clear();

		/// Applies the currently pending changes, is otherwise done automatically when any update is called.
		/// 
		void ApplyPendingChanges();

		/// Register the state so that it may later be able to be managed on the stack using ID (push, pop, etc.).
		/// 
		/// \param StateID: ID of the provided state
		/// \param Args: Optional constructor arguments
		/// 
		template<std::derived_from<State> S, typename... Args>
			requires std::constructible_from<S, const T&, const IDType&, StateStack&, Args...>
		void RegisterState(const IDType& aStateID, Args&&... someArgs)
		{
			// we must copy state id when passed to lambda, in the case that it may be destroyed later
			myFactory[aStateID] = [this, aStateID, ...args = std::forward<Args>(someArgs)]
			{
				return std::make_unique<S>(myContext, aStateID, *this, args...);
			};
		}

	protected:
		enum class Action
		{
			Push,
			Pop,
			Erase,
			Move,
			Clear
		};

		struct PendingChange
		{
			explicit PendingChange(const Action& aAction, const IDType& aStateID, std::size_t aIndex);
			explicit PendingChange(const Action& aAction, const IDType& aStateID);
			explicit PendingChange(const Action& aAction);

			Action		action;
			IDType		stateID;
			std::size_t index;
		};

		using StatePtr		= typename State::Ptr;
		using StateFunc		= typename State::Func;
		using Stack			= std::vector<StatePtr>;
		using Factory		= std::unordered_map<IDType, StateFunc, Hash>;
		using PendingList	= std::vector<PendingChange>;

		auto CreateState(const IDType& aStateID) -> StatePtr;

		T			myContext;
		Stack		myStack;
		Factory		myFactory;
		PendingList myPendingList;
	};

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline StateStack<T, IDType, Hash>::State::State(const T& aContext, const IDType& aID, StateStack& aStateStack)
		: myContext(aContext)
		, myID(aID)
		, myStateStack(&aStateStack)
	{

	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline const IDType& StateStack<T, IDType, Hash>::State::GetID() const noexcept
	{
		return myID;
	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::State::GetContext() const -> const T&
	{
		return myContext;
	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::State::GetContext() -> T&
	{
		return myContext;
	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::State::GetStack() const -> const StateStack&
	{
		return *myStateStack;
	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::State::GetStack() -> StateStack&
	{
		return *myStateStack;
	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline StateStack<T, IDType, Hash>::StateStack(const T& aContext)
		: myContext(aContext)
		, myStack()
		, myFactory()
		, myPendingList()
	{

	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::operator[](std::size_t aIndex) const -> const State&
	{
		return GetState(aIndex);
	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::operator[](std::size_t aIndex) -> State&
	{
		return GetState(aIndex);
	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::GetState(std::size_t aIndex) const -> const State&
	{
		return *myStack[aIndex];
	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::GetState(std::size_t aIndex) -> State&
	{
		return *myStack[aIndex];
	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::GetStateByID(const IDType& aStateID) const -> const State*
	{
		auto it = std::find_if(myStack.begin(), myStack.end(),
			[&aStateID](const StatePtr& aPtr)
			{
				return aPtr->GetID() == aStateID;
			});

		if (it == myStack.end())
		{
			return nullptr;
		}

		return it->get();
	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::GetStateByID(const IDType& aStateID) -> State*
	{
		return const_cast<State*>(std::as_const(*this).GetStateByID(aStateID));
	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::GetTopState() const -> const State&
	{
		return *myStack.back();
	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::GetTopState() -> State&
	{
		return *myStack.back();
	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::GetBotState() const -> const State&
	{
		return *myStack.front();
	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::GetBotState() -> State&
	{
		return *myStack.front();
	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline std::size_t StateStack<T, IDType, Hash>::Count() const noexcept
	{
		return myStack.size();
	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline bool StateStack<T, IDType, Hash>::IsEmpty() const noexcept
	{
		return myStack.empty();
	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void StateStack<T, IDType, Hash>::Update(Timer& aTimer)
	{
		for (auto it = myStack.rbegin(); it != myStack.rend(); ++it)
		{
			if (!(*it)->Update(aTimer))
				break;
		}

		ApplyPendingChanges();
	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void StateStack<T, IDType, Hash>::Push(const IDType& aStateID)
	{
		myPendingList.emplace_back(Action::Push, aStateID);
	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void StateStack<T, IDType, Hash>::Erase(const IDType& aStateID)
	{
		myPendingList.emplace_back(Action::Erase, aStateID);
	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void StateStack<T, IDType, Hash>::Move(const IDType& aStateID, std::size_t aNewIndex)
	{
		myPendingList.emplace_back(Action::Move, aStateID, aNewIndex);
	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void StateStack<T, IDType, Hash>::Pop()
	{
		myPendingList.emplace_back(Action::Pop, IDType());
	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void StateStack<T, IDType, Hash>::Clear()
	{
		myPendingList.emplace_back(Action::Clear, IDType());
	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void StateStack<T, IDType, Hash>::ApplyPendingChanges()
	{
		const auto PopState = [this]
		{
			assert(!myStack.empty() && "Cannot pop an empty stack!");

			myStack.back()->OnDestroy();
			myStack.pop_back();

			if (!myStack.empty())
			{
				myStack.back()->OnActivate();
			}
		};

		for (const PendingChange& change : myPendingList)
		{
			switch (change.action)
			{
				case Action::Push:
				{
					if (!myStack.empty())
					{
						myStack.back()->OnDeactivate();
					}

					StatePtr newState = CreateState(change.stateID);
					newState->OnActivate();

					myStack.emplace_back(std::move(newState));

					break;
				}
				case Action::Pop:
				{
					PopState();
					break;
				}
				case Action::Erase:
				{
					auto it = std::find_if(myStack.begin(), myStack.end(),
						[&change](const StatePtr& aPtr)
						{
							return aPtr->GetID() == change.stateID;
						});

					if (it == myStack.end())
						break;

					if (it != myStack.end() - 1) // if not last
					{
						(*it)->OnDestroy();
						myStack.erase(it);
					}
					else
					{
						PopState(); // if this is the last
					}

					break;
				}
				case Action::Move:
				{
					auto it = std::find_if(myStack.begin(), myStack.end(),
						[&change](const StatePtr& aPtr)
						{
							return aPtr->GetID() == change.stateID;
						});

					if (it == myStack.end())
						break;

					const std::size_t currentIndex = std::distance(it, myStack.begin());

					if (currentIndex == change.index) // nothing to move anyways
						break;

					if (change.index == myStack.size() - 1)
					{
						myStack.back()->OnDeactivate();
						(*it)->OnActivate();
					}

					ctr::MoveTo(myStack, currentIndex, change.index);

					break;
				}
				case Action::Clear:
				{
					for (StatePtr& state : myStack)
					{
						state->OnDestroy();
					}

					myStack.clear();

					break;
				}
				default:
					throw std::runtime_error("Invalid action");
			}
		}

		myPendingList.clear();
	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline StateStack<T, IDType, Hash>::PendingChange::PendingChange(const Action& aAction, const IDType& aStateID, std::size_t aIndex)
		: action(aAction), stateID(aStateID), index(aIndex)
	{

	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline StateStack<T, IDType, Hash>::PendingChange::PendingChange(const Action& aAction, const IDType& aStateID)
		: PendingChange(aAction, aStateID, 0)
	{

	}
	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline StateStack<T, IDType, Hash>::PendingChange::PendingChange(const Action& aAction)
		: PendingChange(aAction, IDType{}, 0)
	{

	}

	template<typename T, typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline auto StateStack<T, IDType, Hash>::CreateState(const IDType& aStateID) -> StatePtr
	{
		const auto it = myFactory.find(aStateID);
		if (it == myFactory.end())
		{
			throw std::runtime_error("State could not be found");
		}

		auto statePtr = it->second();
		statePtr->OnCreate();

		return statePtr;
	}
}