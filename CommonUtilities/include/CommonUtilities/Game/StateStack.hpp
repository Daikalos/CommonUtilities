#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Config.h>

#include "State.hpp"

namespace CommonUtilities
{
	/// StateStack is a simple container for states.
	/// 
	/// \param T: Application context which usually contains pointers to important objects (e.g., input)
	/// \param IDType: Type of the ID used to manage states
	/// 
	template<typename T, typename IDType = std::uint32_t>
	class StateStack : private NonCopyable
	{
	public:
		StateStack(const T& aContext);

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
		void Render() const;

		/// Push a state to the top of the stack.
		/// 
		/// \param aStateID: ID of the state to add
		/// 
		void Push(const IDType& aStateID);

		/// Erases the first instance of the state with the provided ID.
		/// 
		/// \param aStateID: ID of the state to erase
		/// 
		void Erase(const IDType& aStateID);

		/// Pops the state currently at the top of the stack.
		/// 
		void Pop();

		/// Clears the stack.
		/// 
		void Clear();

		template<std::derived_from<State> S, typename... Args>
			requires std::constructible_from<T, IDType, StateStack&, T&, Args...>
		void RegisterState(const IDType& aStateID, Args&&... someArgs)
		{
			myFactory[aStateID] = [this, &aStateID, ...args = std::forward<Args>(someArgs)]
			{
				return std::make_unique<S>(aStateID, *this, args...);
			};
		}

	private:
		enum class Action
		{
			Push,
			Pop,
			Erase,
			Clear
		};

		struct PendingChange
		{
			explicit PendingChange(const Action& aAction, const IDType& aStateID);

			Action action;
			IDType stateID;
		};

		using StatePtr		= typename State<T, IDType>::Ptr;
		using StateFunc		= typename State<T, IDType>::Func;
		using Stack			= std::vector<StatePtr>;
		using Factory		= std::unordered_map<IDType, StateFunc>;
		using PendingList	= std::vector<PendingChange>;

		auto CreateState(const IDType& aStateID) -> StatePtr;
		void ApplyPendingChanges();

		T			myContext;
		Stack		myStack;
		Factory		myFactory;
		PendingList myPendingList;
		bool		myPaused;

		friend class State<T, IDType>;
	};

	template<typename T, typename IDType>
	inline StateStack<T, IDType>::StateStack(const T& aContext)
		: myContext(aContext) { }

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
	inline void StateStack<T, IDType>::Render() const
	{
		for (StatePtr& state : myStack)
		{
			state->Render();
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
	inline void StateStack<T, IDType>::Pop()
	{
		myPendingList.emplace_back(Action::Pop, -1);
	}
	template<typename T, typename IDType>
	inline void StateStack<T, IDType>::Clear()
	{
		myPendingList.emplace_back(Action::Clear, -1);
	}

	template<typename T, typename IDType>
	inline StateStack<T, IDType>::PendingChange::PendingChange(const Action& aAction, const IDType& aStateID)
		: action(aAction), stateID(aStateID)
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

					myStack.emplace_back(CreateState(change.stateID));
					myStack.back()->OnCreate();
					myStack.back()->OnActivate();

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
}