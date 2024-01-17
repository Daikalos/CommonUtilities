#pragma once

#include <concepts>
#include <vector>
#include <memory>
#include <ranges>
#include <numeric>
#include <cassert>
#include <tuple>

#include <CommonUtilities/Config.h>
#include <CommonUtilities/System/IDGenerator.h>
#include <CommonUtilities/Utility/ContainerUtils.hpp>

namespace CommonUtilities
{
	/// BaseEntity is an attempt to model a similar system to Unity's GameObjects. Each entity simply contains
	/// a list of components that you may access and modify through various operations. You will have to specialize
	/// this class for your environment, BaseEntity tries to be as generic and simple as possible.
	/// 
	/// \param C: Base component of BaseEntity that other components derive from
	/// 
	template<class C>
	class BaseEntity
	{
	public:
		using ComponentPtr		= std::unique_ptr<C>;
		using ComponentIDType	= std::size_t;

		constexpr BaseEntity() = default;
		constexpr virtual ~BaseEntity() = 0;

		/// Retrieves a component from entity.
		/// 
		/// \return Const reference to component
		/// 
		template<typename T> requires std::derived_from<T, C>
		NODISC constexpr const T& GetComponent() const;

		/// Retrieves a component from entity.
		/// 
		/// \return Reference to component
		/// 
		template<typename T> requires std::derived_from<T, C>
		NODISC constexpr T& GetComponent();

		/// Tries to retrieve a component from entity.
		/// 
		/// \return Const pointer to component, nullptr if not found
		/// 
		template<typename T> requires std::derived_from<T, C>
		NODISC constexpr const T* TryGetComponent() const;

		/// Tries to retrieve a component from entity.
		/// 
		/// \return Pointer to component, nullptr if not found
		/// 
		template<typename T> requires std::derived_from<T, C>
		NODISC constexpr T* TryGetComponent();

		/// \return Whether specified component is active
		/// 
		template<typename T> requires std::derived_from<T, C>
		NODISC constexpr bool IsComponentActive() const;

		/// \return Whether current entity has specified component
		/// 
		template<typename T> requires std::derived_from<T, C>
		NODISC constexpr bool HasComponent() const;

		/// Attempts to add a component to entity.
		/// 
		/// \param Args: Optional constructor parameters for component
		/// 
		/// \return Pointer to component, nullptr if it already has the component
		/// 
		template<typename T, typename... Args> requires std::derived_from<T, C> && std::constructible_from<T, Args...>
		constexpr T* AddComponent(Args&&... someArgs);

		/// Attempts to add multiple components to entity.
		/// 
		template<typename... Ts> requires (std::derived_from<Ts, C> && ...)
		constexpr void AddComponents();

		/// Attempts to remove a component from entity.
		/// 
		/// \param MaintainOrder: Maintain order in the component list after removal, false does a cyclic erasure
		/// 
		/// \return Whether removal was succesful
		/// 
		template<typename T, bool MaintainOrder = false> requires std::derived_from<T, C>
		constexpr bool RemoveComponent();

		/// Attempts to remove multiple components from entity.
		/// 
		template<typename... Ts, bool MaintainOrder = false> requires (std::derived_from<Ts, C> && ...)
		constexpr void RemoveComponents();

		/// Sets the specified component to a different value.
		/// 
		/// \param Args: Optional constructor parameters for new component
		/// 
		/// \return Reference to new component
		/// 
		template<typename T, typename... Args> requires std::derived_from<T, C> && std::constructible_from<T, Args...>
		constexpr T& SetComponent(Args&&... someArgs);

		/// Attempts to set the specified component to a different value.
		/// 
		/// \param Args: Optional constructor parameters for new component
		/// 
		/// \return Pointer to new component, nullptr if not found
		/// 
		template<typename T, typename... Args> requires std::derived_from<T, C> && std::constructible_from<T, Args...>
		constexpr T* TrySetComponent(Args&&... someArgs);

		/// Sorts the components in the entity.
		/// 
		/// \param Comp: Function used to compare components
		/// 
		template<typename Comp>
		constexpr void SortComponents(Comp&& aComparison);

		/// Pre-allocates memory for the components.
		/// 
		/// \param Capacity: Number of components to pre-allocate memory for.
		/// 
		constexpr void ReserveComponents(std::size_t aCapacity);

		/// Removes all components from entity.
		/// 
		constexpr void ClearComponents() noexcept;

		constexpr bool IsComponentsEmpty() const noexcept;
		constexpr std::size_t ComponentsCount() const noexcept;

		/// Sets the component to be active or not. Will only affect the component based on how you choose 
		/// to use the flag. The only thing it does now is prevent a particular component being refered to 
		/// when ForEach() is called.
		/// 
		/// \param Flag: New active state
		/// 
		template<typename T> requires std::derived_from<T, C>
		constexpr void SetComponentActive(bool aFlag);

		/// Runs a function for every component in the entity.
		/// 
		/// \param Func: Function to run
		/// 
		template<typename Func> requires HasParametersDecay<Func, C>
		constexpr void ForEachComponent(Func&& aFunc) const;

		/// Retrieves a component from a static list of components.
		/// 
		/// \return Reference to component
		/// 
		template<typename T> requires std::derived_from<T, C>
		NODISC constexpr static T& GetStaticComponent();

		/// Retrieves a component from a static list of components.
		/// 
		/// \return Const reference to component
		/// 
		template<typename T> requires std::derived_from<T, C>
		NODISC constexpr static T* TryGetStaticComponent();

		/// \return Whether the static list of components contain the specified component
		/// 
		template<typename T> requires std::derived_from<T, C>
		NODISC constexpr static bool HasStaticComponent();

		/// Attempts to add a component to the static list of components.
		/// 
		/// \param Args: Optional constructor parameters for component
		/// 
		/// \return Pointer to component, nullptr if it already has the component
		/// 
		template<typename T, typename... Args> requires std::derived_from<T, C> && std::constructible_from<T, Args...>
		constexpr static T* AddStaticComponent(Args&&... someArgs);

		/// Attempts to remove a component from the static list of components.
		/// 
		/// \param MaintainOrder: Maintain order in the component list after removal, false does a cyclic erasure
		/// 
		/// \return Whether removal was succesful
		/// 
		template<typename T, bool MaintainOrder = false> requires std::derived_from<T, C>
		constexpr static bool RemoveStaticComponent();

		/// Removes all static components.
		/// 
		constexpr static void ClearStaticComponents() noexcept;

	protected:
		std::vector<ComponentPtr>		myComponents;
		std::vector<ComponentIDType>	myComponentIDs;
		std::vector<bool>				myActiveComponents;

		static std::vector<ComponentPtr> myStaticComponents;
		static std::vector<ComponentIDType> myStaticComponentIDs;
	};

	template<class C>
	constexpr BaseEntity<C>::~BaseEntity() = default;

	template<class C>
	inline std::vector<typename BaseEntity<C>::ComponentPtr> BaseEntity<C>::myStaticComponents;

	template<class C>
	inline std::vector<typename BaseEntity<C>::ComponentIDType> BaseEntity<C>::myStaticComponentIDs;

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	constexpr const T& BaseEntity<C>::GetComponent() const
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myComponentIDs, componentID);
		const auto pos = std::distance(it, myComponentIDs.begin());

		return *myComponents[pos];
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	constexpr T& BaseEntity<C>::GetComponent()
	{
		return const_cast<T&>(std::as_const(*this).template GetComponent<T>()); // const_cast to avoid code duplication
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	constexpr const T* BaseEntity<C>::TryGetComponent() const
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myComponentIDs, componentID); it != myComponentIDs.end())
		{
			const auto pos = std::distance(it, myComponentIDs.begin());
			return myComponents[pos].get();
		}

		return nullptr;
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	constexpr T* BaseEntity<C>::TryGetComponent()
	{
		return const_cast<T*>(std::as_const(*this).template TryGetComponent<T>());
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	constexpr bool BaseEntity<C>::IsComponentActive() const
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myComponentIDs, componentID);
		if (it != myComponentIDs.end())
		{
			const auto pos = std::distance(it, myComponentIDs.begin());
			return myActiveComponents[pos];
		}

		return false;
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	constexpr bool BaseEntity<C>::HasComponent() const
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myComponentIDs, componentID);
		return it != myComponentIDs.end();
	}

	template<class C>
	template<typename T, typename... Args> requires std::derived_from<T, C>&& std::constructible_from<T, Args...>
	constexpr T* BaseEntity<C>::AddComponent(Args&&... someArgs)
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myComponentIDs, componentID); it != myComponentIDs.end())
		{
			return nullptr;
		}

		ComponentPtr component = std::make_unique<C>(std::forward<Args>(someArgs)...);

		myComponents.emplace_back(std::move(component));
		myComponentIDs.emplace_back(componentID);
		myActiveComponents.emplace_back(true);

		return myComponents.back().get();
	}

	template<class C>
	template<typename... Ts> requires (std::derived_from<Ts, C> && ...)
	constexpr void BaseEntity<C>::AddComponents()
	{
		(AddComponent<Ts>(), ...);
	}

	template<class C>
	template<typename T, bool MaintainOrder> requires std::derived_from<T, C>
	constexpr bool BaseEntity<C>::RemoveComponent()
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myComponentIDs, componentID); it != myComponentIDs.end())
		{
			const auto pos = std::distance(it, myComponentIDs.begin());

			if constexpr (MaintainOrder)
			{
				myComponents.erase(myComponents.begin() + pos);
				myComponentIDs.erase(myComponentIDs.begin() + pos);
				myActiveComponents.erase(myActiveComponents.begin() + pos);
			}
			else
			{
				ctr::EraseCyclicAt(myComponents, pos);
				ctr::EraseCyclicAt(myComponentIDs, pos);
				ctr::EraseCyclicAt(myActiveComponents, pos);
			}

			return true;
		}

		return false;
	}

	template<class C>
	template<typename... Ts, bool MaintainOrder> requires (std::derived_from<Ts, C> && ...)
	constexpr void BaseEntity<C>::RemoveComponents()
	{
		(RemoveComponent<Ts, MaintainOrder>, ...);
	}

	template<class C>
	template<typename T, typename... Args> requires std::derived_from<T, C> && std::constructible_from<T, Args...>
	constexpr T& BaseEntity<C>::SetComponent(Args&&... someArgs)
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myComponentIDs, componentID);
		const auto pos = std::distance(it, myComponentIDs.begin());

		return (*myComponents[pos] = T{ std::forward<Args>(someArgs)... });
	}

	template<class C>
	template<typename T, typename... Args> requires std::derived_from<T, C>&& std::constructible_from<T, Args...>
	constexpr T* BaseEntity<C>::TrySetComponent(Args&&... someArgs)
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myComponentIDs, componentID);
		if (it != myComponentIDs.end())
		{
			const auto pos = std::distance(it, myComponentIDs.begin());

			T* compPtr = myComponents[pos].get();
			*compPtr = T{ std::forward<Args>(someArgs)... };

			return compPtr;
		}

		return nullptr;
	}

	template<class C>
	template<typename Comp>
	constexpr void BaseEntity<C>::SortComponents(Comp&& aCompare)
	{
		std::vector<std::size_t> indices(myComponents.size());
		std::iota(indices.begin(), indices.end(), 0);

		std::ranges::sort(indices,
			[this, comp = std::forward<Comp>(aCompare)](std::size_t aFirst, std::size_t aSecond)
			{
				return comp(*myComponents[aFirst], *myComponents[aSecond]);
			});

		ctr::ApplyPermutation(myComponents, indices);
		ctr::ApplyPermutation(myComponentIDs, indices);
		ctr::ApplyPermutation(myActiveComponents, indices);
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	constexpr void BaseEntity<C>::SetComponentActive(bool aFlag)
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myComponentIDs, componentID); it != myComponentIDs.end())
		{
			const auto pos = std::distance(it, myComponentIDs.begin());
			myActiveComponents[pos] = aFlag;
		}
	}

	template<class C>
	constexpr void BaseEntity<C>::ReserveComponents(std::size_t aCapacity)
	{
		myComponents.reserve(aCapacity);
		myComponentIDs.reserve(aCapacity);
		myActiveComponents.reserve(aCapacity);
	}

	template<class C>
	constexpr void BaseEntity<C>::ClearComponents() noexcept
	{
		myComponents.clear();
		myComponentIDs.clear();
		myActiveComponents.clear();
	}

	template<class C>
	constexpr bool BaseEntity<C>::IsComponentsEmpty() const noexcept
	{
		return myComponents.empty();
	}

	template<class C>
	constexpr std::size_t BaseEntity<C>::ComponentsCount() const noexcept
	{
		return myComponents.size();
	}

	template<class C>
	template<typename Func> requires HasParametersDecay<Func, C>
	constexpr void BaseEntity<C>::ForEachComponent(Func&& aFunc) const
	{
		for (std::size_t index = 0; index < myComponents.size(); ++index)
		{
			if (!myActiveComponents[index])
				continue;

			aFunc(*myComponents[index]);
		}
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	constexpr T& BaseEntity<C>::GetStaticComponent()
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myStaticComponentIDs, componentID);
		const auto pos = std::distance(it, myStaticComponentIDs.begin());

		return *myStaticComponents[pos];
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	constexpr T* BaseEntity<C>::TryGetStaticComponent()
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myStaticComponentIDs, componentID); it != myStaticComponentIDs.end())
		{
			const auto pos = std::distance(it, myStaticComponentIDs.begin());
			return myStaticComponents[pos].get();
		}

		return nullptr;
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	constexpr bool BaseEntity<C>::HasStaticComponent()
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myStaticComponentIDs, componentID);
		return it != myStaticComponentIDs.end();
	}
	template<class C>
	template<typename T, typename... Args> requires std::derived_from<T, C>&& std::constructible_from<T, Args...>
	constexpr T* BaseEntity<C>::AddStaticComponent(Args&&... someArgs)
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myStaticComponentIDs, componentID); it != myStaticComponentIDs.end())
		{
			return nullptr;
		}

		ComponentPtr component = std::make_unique<C>(std::forward<Args>(someArgs)...);

		myStaticComponents.emplace_back(std::move(component));
		myStaticComponentIDs.emplace_back(componentID);

		return myStaticComponents.back().get();
	}
	template<class C>
	template<typename T, bool MaintainOrder> requires std::derived_from<T, C>
	constexpr bool BaseEntity<C>::RemoveStaticComponent()
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myStaticComponentIDs, componentID); it == myStaticComponentIDs.end())
		{
			const auto pos = std::distance(it, myStaticComponentIDs.begin());

			if constexpr (MaintainOrder)
			{
				myStaticComponents.erase(myStaticComponents.begin() + pos);
				myStaticComponentIDs.erase(myStaticComponentIDs.begin() + pos);
			}
			else
			{
				ctr::EraseCyclicAt(myStaticComponents, pos);
				ctr::EraseCyclicAt(myStaticComponentIDs, pos);
			}

			return true;
		}

		return false;
	}

	template<class C>
	constexpr void BaseEntity<C>::ClearStaticComponents() noexcept
	{
		myStaticComponents.clear();
		myStaticComponentIDs.clear();
	}
}
