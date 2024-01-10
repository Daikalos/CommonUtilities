#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

#include <CommonUtilities/Utility/ContainerUtils.hpp>
#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/System/Timer.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	/// StateStack is a simple container for states that are managed similarly to a stack. 
	/// States should be derived like this: "class Foo : public StateStack<IDType, Hash>::State { }".
	/// StateStack is furthermore virtual so you may specialize it for your environment.
	/// 
	/// \param IDType: Type of the ID used to manage states
	/// \param Hash: Function that generates the hash for IDType
	/// 
	template<typename IDType = std::uint32_t, typename Hash = std::hash<IDType>> requires IsHashable<Hash, IDType>
	class StateStack : private NonCopyable
	{
	public:
		class State
		{
		public:
			using Ptr	= std::unique_ptr<State>;
			using Func	= std::function<Ptr()>;

			State(const IDType& aID, StateStack& aStateStack);
			virtual ~State() = default;

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
			NODISC auto GetStack() const -> const StateStack&;
			NODISC auto GetStack() -> StateStack&;

		private:
			IDType		myID;
			StateStack*	myStateStack;
		};

		StateStack();
		virtual ~StateStack() = default;

		NODISC auto operator[](std::size_t aIndex) const -> const State&;
		NODISC auto operator[](std::size_t aIndex) -> State&;

		NODISC auto GetState(std::size_t aIndex) const -> const State&;
		NODISC auto GetState(std::size_t aIndex) -> State&;

		NODISC std::size_t Count() const noexcept;
		NODISC bool IsEmpty() const noexcept;

		void Update(Timer& aTimer);

		/// Push a state to the top of the stack.
		/// 
		/// \param aStateID: ID of the state to add
		/// 
		void Push(const IDType& aStateID);

		/// Erases the first instance found with the provided ID.
		/// 
		/// \param aStateID: ID of the state to erase
		/// 
		void Erase(const IDType& aStateID);

		/// Moves the first instance found with the provided ID to the new position.
		/// 
		/// \param aStateID: ID of the state to erase
		/// \param aNewIndex: The new position of the state
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

		template<std::derived_from<State> S, typename... Args>
			requires std::constructible_from<S, const IDType&, StateStack&, Args...>
		void RegisterState(const IDType& aStateID, Args&&... someArgs)
		{
			myFactory[aStateID] = [this, &aStateID, ...args = std::forward<Args>(someArgs)]
			{
				return std::make_unique<S>(aStateID, *this, args...);
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

		Stack		myStack;
		Factory		myFactory;
		PendingList myPendingList;
	};

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline StateStack<IDType, Hash>::State::State(const IDType& aID, StateStack& aStateStack)
		: myID(aID), myStateStack(&aStateStack)
	{

	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline const IDType& StateStack<IDType, Hash>::State::GetID() const noexcept
	{
		return myID;
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateStack<IDType, Hash>::State::GetStack() const -> const StateStack&
	{
		return *myStateStack;
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateStack<IDType, Hash>::State::GetStack() -> StateStack&
	{
		return *myStateStack;
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline StateStack<IDType, Hash>::StateStack()
		: myStack()
		, myFactory()
		, myPendingList()
	{

	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateStack<IDType, Hash>::operator[](std::size_t aIndex) const -> const State&
	{
		return GetState(aIndex);
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateStack<IDType, Hash>::operator[](std::size_t aIndex) -> State&
	{
		return GetState(aIndex);
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateStack<IDType, Hash>::GetState(std::size_t aIndex) const -> const State&
	{
		return *myStack[aIndex];
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateStack<IDType, Hash>::GetState(std::size_t aIndex) -> State&
	{
		return *myStack[aIndex];
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline std::size_t StateStack<IDType, Hash>::Count() const noexcept
	{
		return myStack.size();
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline bool StateStack<IDType, Hash>::IsEmpty() const noexcept
	{
		return myStack.empty();
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline void StateStack<IDType, Hash>::Update(Timer& aTimer)
	{
		for (auto it = myStack.rbegin(); it != myStack.rend(); ++it)
		{
			if (!(*it)->Update(aTimer))
				break;
		}

		ApplyPendingChanges();
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline void StateStack<IDType, Hash>::Push(const IDType& aStateID)
	{
		myPendingList.emplace_back(Action::Push, aStateID);
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline void StateStack<IDType, Hash>::Erase(const IDType& aStateID)
	{
		myPendingList.emplace_back(Action::Erase, aStateID);
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline void StateStack<IDType, Hash>::Move(const IDType& aStateID, std::size_t aNewIndex)
	{
		myPendingList.emplace_back(Action::Move, aStateID, aNewIndex);
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline void StateStack<IDType, Hash>::Pop()
	{
		myPendingList.emplace_back(Action::Pop, IDType());
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline void StateStack<IDType, Hash>::Clear()
	{
		myPendingList.emplace_back(Action::Clear, IDType());
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline void StateStack<IDType, Hash>::ApplyPendingChanges()
	{
		const auto PopState = [this]()
		{
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
					newState->OnCreate();
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

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline StateStack<IDType, Hash>::PendingChange::PendingChange(const Action& aAction, const IDType& aStateID, std::size_t aIndex)
		: action(aAction), stateID(aStateID), index(aIndex)
	{

	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline StateStack<IDType, Hash>::PendingChange::PendingChange(const Action& aAction, const IDType& aStateID)
		: PendingChange(aAction, aStateID, 0)
	{

	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline StateStack<IDType, Hash>::PendingChange::PendingChange(const Action& aAction)
		: PendingChange(aAction, IDType, 0)
	{

	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline auto StateStack<IDType, Hash>::CreateState(const IDType& aStateID) -> StatePtr
	{
		const auto it = myFactory.find(aStateID);
		if (it == myFactory.end())
		{
			throw std::runtime_error("State could not be found");
		}

		return it->second();
	}
}