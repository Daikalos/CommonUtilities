#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <Windows.h>

#include <CommonUtilities/Utility/ContainerUtils.hpp>
#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/System/Timer.h>
#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	/// StateStack is a simple container for states managed similarly to a stack. 
	/// States should be derived like this "class Foo : public StateStack<T, IDType>::State".
	/// 
	/// \param T: Application context which usually contains pointers to important objects (e.g., input)
	/// \param IDType: Type of the ID used to manage states
	/// 
	template<typename T, typename IDType = std::uint32_t>
	class StateStack : private NonCopyable
	{
	public:
		class State
		{
		public:
			using Ptr		= std::unique_ptr<State>;
			using Func		= std::function<Ptr()>;
			using Context	= std::remove_const_t<std::remove_reference_t<T>>;

			State(const IDType& aID, StateStack<T, IDType>& aStateStack, const Context& aContext);
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
			virtual bool FixedUpdate(Timer& aTimer) { return true; }
			virtual bool PostUpdate(Timer& aTimer)	{ return true; }
			virtual void Render(Timer& aTimer) = 0;

		protected:
			NODISC const StateStack<T, IDType>& GetStack() const;
			NODISC StateStack<T, IDType>& GetStack();

			NODISC auto GetContext() const -> const Context&;
			NODISC auto GetContext() -> Context&;

		private:
			IDType					myID;
			StateStack<T, IDType>*	myStateStack;
			Context					myContext;
		};

		StateStack(const typename State::Context& aContext);

		NODISC auto operator[](std::size_t aIndex) const -> const State&;
		NODISC auto operator[](std::size_t aIndex) -> State&;

		NODISC auto GetState(std::size_t aIndex) const -> const State&;
		NODISC auto GetState(std::size_t aIndex) -> State&;

		NODISC std::size_t Count() const noexcept;
		NODISC bool IsEmpty() const noexcept;
		NODISC bool IsPaused() const noexcept;

		/// Determines whether states should be updated, rendering will still run as normal
		/// 
		void SetPaused(bool aFlag);

		bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam);

		void Init();
		void PreUpdate(Timer& aTimer);
		void Update(Timer& aTimer);
		void FixedUpdate(Timer& aTimer);
		void PostUpdate(Timer& aTimer);
		void Render(Timer& aTimer);

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
			requires std::constructible_from<S, const IDType&, StateStack&, const typename State::Context&, Args...>
		void RegisterState(const IDType& aStateID, Args&&... someArgs)
		{
			myFactory[aStateID] = [this, &aStateID, ...args = std::forward<Args>(someArgs)]
			{
				return std::make_unique<S>(aStateID, *this, myContext, args...);
			};
		}

	private:
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

		using Context		= typename State::Context;
		using StatePtr		= typename State::Ptr;
		using StateFunc		= typename State::Func;
		using Stack			= std::vector<StatePtr>;
		using Factory		= std::unordered_map<IDType, StateFunc>;
		using PendingList	= std::vector<PendingChange>;

		auto CreateState(const IDType& aStateID) -> StatePtr;

		Context		myContext;
		Stack		myStack;
		Factory		myFactory;
		PendingList myPendingList;
		bool		myPaused;
	};

	template<typename T, typename IDType>
	inline StateStack<T, IDType>::State::State(const IDType& aID, StateStack<T, IDType>& aStateStack, const Context& aContext)
		: myID(aID), myStateStack(&aStateStack), myContext(aContext)
	{

	}

	template<typename T, typename IDType>
	inline const IDType& StateStack<T, IDType>::State::GetID() const noexcept
	{
		return myID;
	}

	template<typename T, typename IDType>
	inline const StateStack<T, IDType>& StateStack<T, IDType>::State::GetStack() const
	{
		return *myStateStack;
	}
	template<typename T, typename IDType>
	inline StateStack<T, IDType>& StateStack<T, IDType>::State::GetStack()
	{
		return *myStateStack;
	}

	template<typename T, typename IDType>
	inline auto StateStack<T, IDType>::State::GetContext() const -> const Context&
	{
		return myContext;
	}
	template<typename T, typename IDType>
	inline auto StateStack<T, IDType>::State::GetContext() -> Context&
	{
		return myContext;
	}

	template<typename T, typename IDType>
	inline StateStack<T, IDType>::StateStack(const typename State::Context& aContext)
		: myContext(aContext) { }

	template<typename T, typename IDType>
	inline auto StateStack<T, IDType>::operator[](std::size_t aIndex) const -> const State&
	{
		return GetState(aIndex);
	}
	template<typename T, typename IDType>
	inline auto StateStack<T, IDType>::operator[](std::size_t aIndex) -> State&
	{
		return GetState(aIndex);
	}

	template<typename T, typename IDType>
	inline auto StateStack<T, IDType>::GetState(std::size_t aIndex) const -> const State&
	{
		return *myStack[aIndex];
	}
	template<typename T, typename IDType>
	inline auto StateStack<T, IDType>::GetState(std::size_t aIndex) -> State&
	{
		return *myStack[aIndex];
	}

	template<typename T, typename IDType>
	inline std::size_t StateStack<T, IDType>::Count() const noexcept
	{
		return myStack.size();
	}
	template<typename T, typename IDType>
	inline bool StateStack<T, IDType>::IsEmpty() const noexcept
	{
		return myStack.empty();
	}
	template<typename T, typename IDType>
	inline bool StateStack<T, IDType>::IsPaused() const noexcept
	{
		return myPaused;
	}

	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::SetPaused(bool flag)
	{
		myPaused = flag;
	}

	template<typename T, typename IDType>
	inline bool StateStack<T, IDType>::HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam)
	{
		if (myPaused) // not sure if should be here
			return;

		for (auto it = myStack.rbegin(); it != myStack.rend(); ++it)
		{
			if (!(*it)->HandleEvent(aMessage, wParam, lParam))
				break;
		}

		ApplyPendingChanges();
	}

	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::Init()
	{
		for (auto it = myStack.rbegin(); it != myStack.rend(); ++it)
		{
			if (!(*it)->Init())
				break;
		}

		ApplyPendingChanges();
	}

	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::PreUpdate(Timer& aTimer)
	{
		if (myPaused)
			return;

		for (auto it = myStack.rbegin(); it != myStack.rend(); ++it)
		{
			if (!(*it)->PreUpdate(aTimer))
				break;
		}

		ApplyPendingChanges();
	}

	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::Update(Timer& aTimer)
	{
		if (myPaused)
			return;

		for (auto it = myStack.rbegin(); it != myStack.rend(); ++it)
		{
			if (!(*it)->Update(aTimer))
				break;
		}

		ApplyPendingChanges();
	}

	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::FixedUpdate(Timer& aTimer)
	{
		if (myPaused)
			return;

		for (auto it = myStack.rbegin(); it != myStack.rend(); ++it)
		{
			if (!(*it)->FixedUpdate(aTimer))
				break;
		}

		ApplyPendingChanges();
	}

	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::PostUpdate(Timer& aTimer)
	{
		if (myPaused)
			return;

		for (auto it = myStack.rbegin(); it != myStack.rend(); ++it)
		{
			if (!(*it)->PostUpdate(aTimer))
				break;
		}

		ApplyPendingChanges();
	}

	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::Render(Timer& aTimer)
	{
		for (StatePtr& state : myStack)
		{
			state->Render(aTimer);
		}
	}

	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::Push(const IDType& aStateID)
	{
		myPendingList.emplace_back(Action::Push, aStateID);
	}
	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::Erase(const IDType& aStateID)
	{
		myPendingList.emplace_back(Action::Erase, aStateID);
	}
	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::Move(const IDType& aStateID, std::size_t aNewIndex)
	{
		myPendingList.emplace_back(Action::Move, aStateID, aNewIndex);
	}
	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::Pop()
	{
		myPendingList.emplace_back(Action::Pop, IDType());
	}
	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::Clear()
	{
		myPendingList.emplace_back(Action::Clear, IDType());
	}

	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::ApplyPendingChanges()
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

				const auto currentIndex = std::distance(it, myStack.begin());

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

	template<typename T, typename IDType>
	inline StateStack<T, IDType>::PendingChange::PendingChange(const Action& aAction, const IDType& aStateID, std::size_t aIndex)
		: action(aAction), stateID(aStateID), index(aIndex)
	{

	}
	template<typename T, typename IDType>
	inline StateStack<T, IDType>::PendingChange::PendingChange(const Action& aAction, const IDType& aStateID)
		: PendingChange(aAction, aStateID, 0)
	{

	}
	template<typename T, typename IDType>
	inline StateStack<T, IDType>::PendingChange::PendingChange(const Action& aAction)
		: PendingChange(aAction, IDType, 0)
	{

	}

	template<typename T, typename IDType>
	inline auto StateStack<T, IDType>::CreateState(const IDType& aStateID) -> StatePtr
	{
		const auto it = myFactory.find(aStateID);
		if (it == myFactory.end())
		{
			throw std::runtime_error("State could not be found");
		}

		return it->second();
	}
}