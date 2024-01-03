#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Config.h>

#include "State.hpp"

namespace CommonUtilities
{
	template<typename T>
	class StateStack : private NonCopyable
	{
	public:
		StateStack(T& aContext);

		NODISC bool IsEmpty() const noexcept;
		NODISC bool IsPaused() const noexcept;

		void SetPaused(bool aFlag);

		bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam);

		void Init(Timer& aTimer);
		void PreUpdate(Timer& aTimer);
		void Update(Timer& aTimer);
		void FixedUpdate(Timer& aTimer);
		void PostUpdate(Timer& aTimer);

		void Render() const;

		void Push(StateID aStateID);
		void Pop();
		void Clear();

		template<std::derived_from<State> S, typename... Args>
			requires std::constructible_from<T, StateID, StateStack&, T&, Args...>
		void RegisterState(StateID aStateID, Args&&... someArgs)
		{
			myFactory[aStateID] = [this, aStateID, ... args = std::forward<Args>(someArgs)]()
			{
				return std::make_unique<S>(aStateID, *this, std::forward<Args>(args)...);
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
			explicit PendingChange(const Action& aAction, StateID aStateID = -1);

			Action action;
			StateID stateID;
		};

		using StatePtr = typename State<T>::Ptr;
		using StateFunc = typename State<T>::Func;

		using Stack = std::vector<StatePtr>;
		using Factory = std::unordered_map<StateID, StateFunc>;

		using PendingList = std::vector<PendingChange>;

		auto CreateState(StateID aStateID) -> StatePtr;
		void ApplyPendingChanges();

		T*			myContext;
		Stack		myStack;
		Factory		myFactory;
		PendingList myPendingList;
		bool		myPaused;

		friend class State<T>;
	};

	template<typename T>
	inline StateStack<T>::StateStack(T& aContext)
		: myContext(&aContext) { }

	template<typename T>
	inline bool StateStack<T>::IsEmpty() const noexcept
	{
		return myStack.empty();
	}
	template<typename T>
	inline bool StateStack<T>::IsPaused() const noexcept
	{
		return myPaused;
	}

	template<typename T>
	inline void StateStack<T>::SetPaused(bool flag)
	{
		myPaused = flag;
	}

	template<typename T>
	inline bool StateStack<T>::HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam)
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

	template<typename T>
	inline void StateStack<T>::Init(Timer& aTimer)
	{
		for (auto it = myStack.rbegin(); it != myStack.rend(); ++it)
		{
			if (!(*it)->Init(aTimer))
				break;
		}

		ApplyPendingChanges();
	}

	template<typename T>
	inline void StateStack<T>::PreUpdate(Timer& aTimer)
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

	template<typename T>
	inline void StateStack<T>::Update(Timer& aTimer)
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

	template<typename T>
	inline void StateStack<T>::FixedUpdate(Timer& aTimer)
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

	template<typename T>
	inline void StateStack<T>::PostUpdate(Timer& aTimer)
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

	template<typename T>
	inline void StateStack<T>::Render() const
	{
		for (StatePtr& state : myStack)
			state->Render();
	}

	template<typename T>
	inline void StateStack<T>::Push(StateID state_id)
	{
		myPendingList.push_back(PendingChange(Action::Push, state_id));
	}
	template<typename T>
	inline void StateStack<T>::Pop()
	{
		myPendingList.push_back(PendingChange(Action::Pop));
	}
	template<typename T>
	inline void StateStack<T>::Clear()
	{
		myPendingList.push_back(PendingChange(Action::Clear));
	}

	template<typename T>
	inline StateStack<T>::PendingChange::PendingChange(const Action& aAction, StateID aStateID)
		: action(aAction), stateID(aStateID)
	{

	}

	template<typename T>
	inline auto StateStack<T>::CreateState(StateID state_id) -> StatePtr
	{
		const auto it = myFactory.find(state_id);
		if (it == myFactory.end())
			throw std::runtime_error("State could not be found");

		return it->second();
	}

	template<typename T>
	inline void StateStack<T>::ApplyPendingChanges()
	{
		const auto PopState = [this]()
		{
			myStack.back()->OnDestroy();
			myStack.pop_back();

			if (!myStack.empty())
				myStack.back()->OnActivate();
		};

		for (const PendingChange& change : myPendingList)
		{
			switch (change.action)
			{
				case Action::Push:
				{
					myStack.emplace_back(CreateState(change.stateID));
					myStack.back()->OnCreate();
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
						state->OnDestroy();

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